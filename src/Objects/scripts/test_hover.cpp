/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "CreatureScript.h"
#include "GameObjectScript.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "TaskScheduler.h"
#include "karazhan.h"

enum Spells
{
    // phase 1
    SPELL_BELLOWING_ROAR        = 39427,
    SPELL_CLEAVE                = 30131,
    SPELL_CHARRED_EARTH         = 30129,
    SPELL_DISTRACTING_ASH       = 30130,
    SPELL_SMOLDERING_BREATH     = 30210,
    SPELL_TAIL_SWEEP            = 25653,
    // phase 2
    SPELL_RAIN_OF_BONES         = 37098,
    SPELL_SMOKING_BLAST         = 37057,
    SPELL_FIREBALL_BARRAGE      = 30282,
    SPELL_SEARING_CINDERS       = 30127,
    SPELL_SUMMON_SKELETON       = 30170
};

enum Says
{
    EMOTE_SUMMON                = 0,
    YELL_AGGRO                  = 1,
    YELL_FLY_PHASE              = 2,
    YELL_LAND_PHASE             = 3,
    EMOTE_BREATH                = 4
};

enum Groups
{
    GROUP_GROUND                = 0,
    GROUP_FLYING                = 1
};

enum Points
{
    POINT_DESPAWN = 10 // Other points used dynamically throughout the script
};

struct test_hover : public BossAI
{
    test_hover(Creature* creature) : BossAI(creature, DATA_NIGHTBANE)
    {
        _movePhase = 0;
    }

    void Reset() override
    {
        BossAI::Reset();

        me->SetSpeed(MOVE_RUN, 2.0f);
        me->SetDisableGravity(false);
        me->SetWalk(false);
        me->setActive(true);

        _flying = false;
        _movement = false;
        Phase = 1;
        _movePhase = 0;

        me->RemoveUnitFlag(UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);

        ScheduleHealthCheckEvent({ 75, 50, 25 }, [&]{
            TakeOff();
        });
    }

    void EnterEvadeMode(EvadeReason why) override
    {
        BossAI::EnterEvadeMode(why);
        me->SetDisableGravity(true);
        me->SendMovementFlagUpdate();
        me->GetMotionMaster()->MoveTakeoff(POINT_DESPAWN, -11127.625f, -2043.02f, 47.075f);
    }

    void JustEngagedWith(Unit* who) override
    {
        BossAI::JustEngagedWith(who);
        me->SetDisableGravity(false);
        me->SetInCombatWithZone();


        Talk(YELL_AGGRO);
    }

    void ScheduleFly()
    {
        scheduler.Schedule(2s, GROUP_FLYING, [this](TaskContext)
        {
            DoResetThreatList();
            // DoCastVictim(SPELL_RAIN_OF_BONES);
        }).Schedule(20s, GROUP_FLYING, [this](TaskContext context)
        {
            DoCastRandomTarget(SPELL_DISTRACTING_ASH);
            context.Repeat(2s); //timer wrong?
        }).Schedule(25s, GROUP_FLYING, [this](TaskContext context)
        {
            //5 seconds added due to double trigger?
            //trigger for timer in original + in rain of bones
            //timers need some investigation
            DoCastVictim(SPELL_SMOKING_BLAST);
            context.Repeat(1500ms); //timer wrong?
        }).Schedule(13s, GROUP_FLYING, [this](TaskContext context)
        {
            context.Repeat(20s);
        });
    }

    void AttackStart(Unit* who) override
    {
        if (!_flying)
            ScriptedAI::AttackStart(who);
    }

    void MoveInLineOfSight(Unit* who) override
    {
        if (!_flying)
            ScriptedAI::MoveInLineOfSight(who);
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (id == POINT_DESPAWN)
        {
            me->DespawnOnEvade();
        }
    }

    void JustSummoned(Creature* summon) override
    {
        summon->AI()->AttackStart(me->GetVictim());
        summons.Summon(summon);
    }

    void TakeOff()
    {
        Talk(YELL_FLY_PHASE);
        scheduler.CancelGroup(GROUP_GROUND);

        me->InterruptSpell(CURRENT_GENERIC_SPELL);
        me->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
        me->SetDisableGravity(true);
        me->GetMotionMaster()->Clear(false);
        me->GetMotionMaster()->MovePoint(0, -11127.625f, -2043.02f, 77.075f);
        _flying = true;

        ScheduleFly();

        //handle landing again
        scheduler.Schedule(25s, 25s, [this](TaskContext)
        {
            Talk(YELL_LAND_PHASE);

            me->GetMotionMaster()->Clear(false);
            me->GetMotionMaster()->MoveLand(3, -11127.625f, -2043.02f, 47.075f);

            _flying = true;
            scheduler.CancelGroup(GROUP_FLYING);
        });
    }

    void UpdateAI(uint32 diff) override
    {
        if (_flying && _movePhase)
        {
            me->SetDisableGravity(false);
            DoResetThreatList();
            me->HandleEmoteCommand(EMOTE_ONESHOT_LAND);
            me->RemoveUnitFlag(UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
            // me->GetMotionMaster()->MovePoint(8, IntroWay[7][0], IntroWay[7][1], IntroWay[7][2]);
            _movePhase = 0;
        }

        if (!UpdateVictim())
            return;

        if (_flying)
            return;

        scheduler.Update(diff);

        //  Phase 1 "GROUND FIGHT"
        if (Phase == 1)
        {
            if (_movement)
            {
                DoStartMovement(me->GetVictim());
                _movement = false;
            }

            DoMeleeAttackIfReady();
        }
    }

private:
    uint32 Phase;

    bool _flying;
    bool _movement;

    uint32 _movePhase;
};

void AddSC_test_hover()
{
    RegisterKarazhanCreatureAI(test_hover);
}
