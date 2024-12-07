
D2MOO_prepare_targets(D2Game)

target_link_libraries(${D2GameImplName}
  PRIVATE
    D2CommonDefinitions
    D2CMP
    D2Common
    D2Lang
    D2Net
    Fog
    winmm # For timeGetTime()
)
target_sources(${D2GameImplName}
  PRIVATE
    include/D2Game.h
)
# Ai
target_sources(${D2GameImplName}
  PRIVATE
    src/AI/AiBaal.cpp
    src/AI/AiGeneral.cpp
    src/AI/AiTactics.cpp
    src/AI/AiThink.cpp
    src/AI/AiUtil.cpp

    include/AI/AiBaal.h
    include/AI/AiGeneral.h
    include/AI/AiStates.h
    include/AI/AiTactics.h
    include/AI/AiThink.h
    include/AI/AiUtil.h
)

# Debug
target_sources(${D2GameImplName}
  PRIVATE
    src/DEBUG/Debug.cpp

    include/DEBUG/Debug.h
)

# Game
target_sources(${D2GameImplName}
  PRIVATE
    src/GAME/Arena.cpp
    src/GAME/CCmd.cpp
    src/GAME/Clients.cpp
    src/GAME/Event.cpp
    src/GAME/Game.cpp
    src/GAME/Level.cpp
    src/GAME/SCmd.cpp
    src/GAME/Targets.cpp
    src/GAME/Task.cpp

    include/GAME/Arena.h
    include/GAME/CCmd.h
    include/GAME/Clients.h
    include/GAME/Event.h
    include/GAME/Game.h
    include/GAME/Level.h
    include/GAME/SCmd.h
    include/GAME/Targets.h
    include/GAME/Task.h
)

# Inventory
target_sources(${D2GameImplName}
  PRIVATE
    src/INVENTORY/InvMode.cpp
    
    include/INVENTORY/InvMode.h
)

# Items
target_sources(${D2GameImplName}
  PRIVATE
    src/ITEMS/ItemMode.cpp
    src/ITEMS/Items.cpp
    src/ITEMS/ItemsMagic.cpp
    
    include/ITEMS/ItemMode.h
    include/ITEMS/Items.h
    include/ITEMS/ItemsMagic.h
)

# Missiles
target_sources(${D2GameImplName}
  PRIVATE
    src/MISSILES/Missiles.cpp
    src/MISSILES/MissMode.cpp
    
    include/MISSILES/Missiles.h
    include/MISSILES/MissMode.h
)

# Monster
target_sources(${D2GameImplName}
  PRIVATE
    src/MONSTER/Monster.cpp
    src/MONSTER/MonsterAI.cpp
    src/MONSTER/MonsterChoose.cpp
    src/MONSTER/MonsterMode.cpp
    src/MONSTER/MonsterMsg.cpp
    src/MONSTER/MonsterRegion.cpp
    src/MONSTER/MonsterSpawn.cpp
    src/MONSTER/MonsterUnique.cpp

    include/MONSTER/Monster.h
    include/MONSTER/MonsterAI.h
    include/MONSTER/MonsterChoose.h
    include/MONSTER/MonsterMode.h    
    include/MONSTER/MonsterMsg.h
    include/MONSTER/MonsterRegion.h
    include/MONSTER/MonsterSpawn.h
    include/MONSTER/MonsterUnique.h
)

# Objects
target_sources(${D2GameImplName}
  PRIVATE
    src/OBJECTS/Objects.cpp
    src/OBJECTS/ObjEval.cpp
    src/OBJECTS/ObjMode.cpp
    src/OBJECTS/ObjRgn.cpp

    include/OBJECTS/Objects.h
    include/OBJECTS/ObjEval.h
    include/OBJECTS/ObjMode.h
    include/OBJECTS/ObjRgn.h
)

# Player
target_sources(${D2GameImplName}
  PRIVATE
    src/PLAYER/PartyScreen.cpp
    src/PLAYER/Player.cpp
    src/PLAYER/PlayerList.cpp
    src/PLAYER/PlayerPets.cpp
    src/PLAYER/PlayerStats.cpp
    src/PLAYER/PlrIntro.cpp
    src/PLAYER/PlrModes.cpp
    src/PLAYER/PlrMsg.cpp
    src/PLAYER/PlrSave.cpp
    src/PLAYER/PlrSave2.cpp
    src/PLAYER/PlrTrade.cpp

    include/PLAYER/PartyScreen.h
    include/PLAYER/Player.h
    include/PLAYER/PlayerList.h
    include/PLAYER/PlayerPets.h
    include/PLAYER/PlayerStats.h
    include/PLAYER/PlrIntro.h
    include/PLAYER/PlrModes.h
    include/PLAYER/PlrMsg.h
    include/PLAYER/PlrSave.h
    include/PLAYER/PlrSave2.h
    include/PLAYER/PlrTrade.h
)

# Packets
target_sources(${D2GameImplName}
  PRIVATE
    include/D2Packet.h
)

# Quests
target_sources(${D2GameImplName}
  PRIVATE
    src/QUESTS/Quests.cpp
    src/QUESTS/QuestsFX.cpp

    include/QUESTS/Quests.h
    include/QUESTS/QuestsFX.h
)

# Quests - Act1
target_sources(${D2GameImplName}
  PRIVATE
    src/QUESTS/ACT1/A1Intro.cpp
    src/QUESTS/ACT1/A1Q0.cpp
    src/QUESTS/ACT1/A1Q1.cpp
    src/QUESTS/ACT1/A1Q2.cpp
    src/QUESTS/ACT1/A1Q3.cpp
    src/QUESTS/ACT1/A1Q4.cpp
    src/QUESTS/ACT1/A1Q5.cpp
    src/QUESTS/ACT1/A1Q6.cpp
    src/QUESTS/ACT1/A1Q7.cpp

    include/QUESTS/ACT1/A1Intro.h
    include/QUESTS/ACT1/A1Q0.h
    include/QUESTS/ACT1/A1Q1.h
    include/QUESTS/ACT1/A1Q2.h
    include/QUESTS/ACT1/A1Q3.h
    include/QUESTS/ACT1/A1Q4.h
    include/QUESTS/ACT1/A1Q5.h
    include/QUESTS/ACT1/A1Q6.h
    include/QUESTS/ACT1/A1Q7.h
)

# Quests - Act2
target_sources(${D2GameImplName}
  PRIVATE
    src/QUESTS/ACT2/A2Intro.cpp
    src/QUESTS/ACT2/A2Q0.cpp
    src/QUESTS/ACT2/A2Q1.cpp
    src/QUESTS/ACT2/A2Q2.cpp
    src/QUESTS/ACT2/A2Q3.cpp
    src/QUESTS/ACT2/A2Q4.cpp
    src/QUESTS/ACT2/A2Q5.cpp
    src/QUESTS/ACT2/A2Q6.cpp
    src/QUESTS/ACT2/A2Q7.cpp
    src/QUESTS/ACT2/A2Q8.cpp

    include/QUESTS/ACT2/A2Intro.h
    include/QUESTS/ACT2/A2Q0.h
    include/QUESTS/ACT2/A2Q1.h
    include/QUESTS/ACT2/A2Q2.h
    include/QUESTS/ACT2/A2Q3.h
    include/QUESTS/ACT2/A2Q4.h
    include/QUESTS/ACT2/A2Q5.h
    include/QUESTS/ACT2/A2Q6.h
    include/QUESTS/ACT2/A2Q7.h
    include/QUESTS/ACT2/A2Q8.h
)

# Quests - Act3
target_sources(${D2GameImplName}
  PRIVATE
    src/QUESTS/ACT3/A3Intro.cpp
    src/QUESTS/ACT3/A3Q0.cpp
    src/QUESTS/ACT3/A3Q1.cpp
    src/QUESTS/ACT3/A3Q2.cpp
    src/QUESTS/ACT3/A3Q3.cpp
    src/QUESTS/ACT3/A3Q4.cpp
    src/QUESTS/ACT3/A3Q5.cpp
    src/QUESTS/ACT3/A3Q6.cpp
    src/QUESTS/ACT3/A3Q7.cpp

    include/QUESTS/ACT3/A3Intro.h
    include/QUESTS/ACT3/A3Q0.h
    include/QUESTS/ACT3/A3Q1.h
    include/QUESTS/ACT3/A3Q2.h
    include/QUESTS/ACT3/A3Q3.h
    include/QUESTS/ACT3/A3Q4.h
    include/QUESTS/ACT3/A3Q5.h
    include/QUESTS/ACT3/A3Q6.h
    include/QUESTS/ACT3/A3Q7.h
)

# Quests - Act4
target_sources(${D2GameImplName}
  PRIVATE
    src/QUESTS/ACT4/A4Q0.cpp
    src/QUESTS/ACT4/A4Q1.cpp
    src/QUESTS/ACT4/A4Q2.cpp
    src/QUESTS/ACT4/A4Q3.cpp
    src/QUESTS/ACT4/A4Q4.cpp

    include/QUESTS/ACT4/A4Q0.h
    include/QUESTS/ACT4/A4Q1.h
    include/QUESTS/ACT4/A4Q2.h
    include/QUESTS/ACT4/A4Q3.h
    include/QUESTS/ACT4/A4Q4.h
)

# Quests - Act5
target_sources(${D2GameImplName}
  PRIVATE
    src/QUESTS/ACT5/A5Intro.cpp
    src/QUESTS/ACT5/A5Q1.cpp
    src/QUESTS/ACT5/A5Q2.cpp
    src/QUESTS/ACT5/A5Q3.cpp
    src/QUESTS/ACT5/A5Q4.cpp
    src/QUESTS/ACT5/A5Q5.cpp
    src/QUESTS/ACT5/A5Q6.cpp

    include/QUESTS/ACT5/A5Intro.h
    include/QUESTS/ACT5/A5Q1.h
    include/QUESTS/ACT5/A5Q2.h
    include/QUESTS/ACT5/A5Q3.h
    include/QUESTS/ACT5/A5Q4.h
    include/QUESTS/ACT5/A5Q5.h
    include/QUESTS/ACT5/A5Q6.h
)

# Unit
target_sources(${D2GameImplName}
  PRIVATE
    src/UNIT/Friendly.cpp
    src/UNIT/Party.cpp
    src/UNIT/SUnit.cpp
    src/UNIT/SUnitDmg.cpp
    src/UNIT/SUnitEvent.cpp
    src/UNIT/SUnitInactive.cpp
    src/UNIT/SUnitMsg.cpp
    src/UNIT/SUnitNpc.cpp
    src/UNIT/SUnitProxy.cpp

    include/UNIT/Friendly.h
    include/UNIT/Party.h
    include/UNIT/SUnit.h
    include/UNIT/SUnitDmg.h
    include/UNIT/SUnitEvent.h
    include/UNIT/SUnitInactive.h
    include/UNIT/SUnitMsg.h
    include/UNIT/SUnitNpc.h
    include/UNIT/SUnitProxy.h
)

# Skills
target_sources(${D2GameImplName}
  PRIVATE
    src/SKILLS/SkillAma.cpp
    src/SKILLS/SkillAss.cpp
    src/SKILLS/SkillBar.cpp
    src/SKILLS/SkillDruid.cpp
    src/SKILLS/SkillItem.cpp
    src/SKILLS/SkillMonst.cpp
    src/SKILLS/SkillNec.cpp
    src/SKILLS/SkillPal.cpp
    src/SKILLS/Skills.cpp
    src/SKILLS/SkillSor.cpp

    include/SKILLS/SkillAma.h
    include/SKILLS/SkillAss.h
    include/SKILLS/SkillBar.h
    include/SKILLS/SkillDruid.h
    include/SKILLS/SkillItem.h
    include/SKILLS/SkillMonst.h
    include/SKILLS/SkillNec.h
    include/SKILLS/SkillPal.h
    include/SKILLS/Skills.h
    include/SKILLS/SkillSor.h
)

if(D2MOO_WITH_STATIC_TESTS)
  target_sources(${D2GameImplName}
    PRIVATE
      statictests/Clients_staticchecks.cpp
      statictests/Quests_staticchecks.cpp
  )
endif()

D2MOO_target_source_group(D2Game)
