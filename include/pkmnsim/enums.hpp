/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt,
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_ENUMS_HPP
#define INCLUDED_PKMNSIM_ENUMS_HPP

#include <string>
#include <vector>

#include <pkmnsim/config.hpp>

namespace pkmnsim
{
    /*,
     * Globally used enums
     */

    namespace Genders
    {
        enum genders
        {
            MALE = 0,
            FEMALE = 1,
            GENDERLESS = 2
        };
    }

    namespace Forms
    {
        namespace Pichu
        {
            enum forms
            {
                NORMAL = 0,
                SPIKY_EARED = 1
            };
        }

        namespace Unown
        {
            enum forms
            {
                A = 1,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
                EXCLAMATION,QUESTION
            };
        }

        namespace Castform
        {
            enum forms
            {
                NORMAL = 0,
                SUNNY = 1,
                RAINY = 2,
                SNOWY = 3
            };
        }

        namespace Deoxys
        {
            enum forms
            {
                NORMAL = 0,
                ATTACK = 1,
                DEFENSE = 2,
                SPEED = 3
            };
        }

        namespace Burmy
        {
            enum forms
            {
                PLANT_CLOAK = 0,
                SANDY_CLOAK = 1,
                TRASH_CLOAK = 2
            };
        }

        namespace Wormadam
        {
            enum forms
            {
                PLANT_CLOAK = 0,
                SANDY_CLOAK = 1,
                TRASH_CLOAK = 2
            };
        }

        namespace Cherrim
        {
            enum forms
            {
                OVERCAST = 0,
                SUNSHINE = 1
            };
        }

        namespace Shellos
        {
            enum forms
            {
                WEST_SEA = 0,
                EAST_SEA = 1
            };
        }

        namespace Gastrodon
        {
            enum forms
            {
                WEST_SEA = 0,
                EAST_SEA = 1
            };
        }

        namespace Rotom
        {
            enum forms
            {
                NORMAL = 0,
                HEAT = 1,
                WASH = 2,
                FROST = 3,
                FAN = 4,
                MOW = 5
            };
        }

        namespace Giratina
        {
            enum forms
            {
                ALTERED = 0,
                ORIGIN = 1
            };
        }

        namespace Shaymin
        {
            enum forms
            {
                LAND = 0,
                SKY = 1
            };
        }

        namespace Arceus
        {
            enum forms
            {
                NORMAL = 0,
                FIRE = 1,
                WATER = 2,
                ELECTRIC = 3,
                GRASS = 4,
                ICE = 5,
                FIGHTING = 6,
                POISON = 7,
                GROUND = 8,
                FLYING = 9,
                PSYCHIC = 10,
                BUG = 11,
                ROCK = 12,
                GHOST = 13,
                DRAGON = 14,
                DARK = 15,
                STEEL = 16
            };
        }

        namespace Basculin
        {
            enum forms
            {
                RED_STRIPED = 0,
                BLUE_STRIPED = 1
            };
        }

        namespace Darmanitan
        {
            enum forms
            {
                STANDARD = 0,
                ZEN = 1
            };
        }

        namespace Deerling
        {
            enum forms
            {
                SPRING = 0,
                SUMMER = 1,
                AUTUMN = 2,
                WINTER = 3
            };
        }

        namespace Sawsbuck
        {
            enum forms
            {
                SPRING = 0,
                SUMMER = 1,
                AUTUMN = 2,
                WINTER = 3
            };
        }

        namespace Tornadus
        {
            enum forms
            {
                INCARNATE = 0,
                THERIAN = 1
            };
        }

        namespace Thundurus
        {
            enum forms
            {
                INCARNATE = 0,
                THERIAN = 1
            };
        }

        namespace Landorus
        {
            enum forms
            {
                INCARNATE = 0,
                THERIAN = 1
            };
        }

        namespace Kyurem
        {
            enum forms
            {
                NORMAL = 0,
                BLACK = 1,
                WHITE = 2
            };
        }

        namespace Keldeo
        {
            enum forms
            {
                ORDINARY = 0,
                RESOLUTE = 1
            };
        }

        namespace Meloetta
        {
            enum forms
            {
                ARIA = 0,
                PIROUETTE = 1
            };
        }

        namespace Genesect
        {
            enum forms
            {
                NORMAL = 0,
                SHOCK_DRIVE = 1,
                BURN_DRIVE = 2,
                CHILL_DRIVE = 3,
                DOUSE_DRIVE = 4
            };
        }
    }

    namespace Move_Classes
    {
        enum move_classes
        {
            NON_DAMAGING = 1,
            PHYSICAL = 2,
            SPECIAL = 3
        };
    }

    namespace Statuses
    {
        enum nonvolatile_statuses
        {
            OK = 0, //No nonvolatile status
            BRN = 1, //Burn
            FRZ = 2, //Frozen
            PRZ = 3, //Paralysis
            PSN = 4, //Poison
            BPN = 5, //Bad Poison
            SLP = 6 //Asleep
        };
    }

    namespace Games
    {
        enum games
        {
            RED = 1,
            BLUE,
            YELLOW,
            GOLD,
            SILVER,
            CRYSTAL,
            RUBY,
            SAPPHIRE,
            EMERALD,
            FIRE_RED,
            LEAF_GREEN,
            DIAMOND,
            PEARL,
            PLATINUM,
            HEART_GOLD,
            SOUL_SILVER,
            BLACK,
            WHITE,
            COLOSSEUM,
            XD,
            BLACK2,
            WHITE2,
            STADIUM,
            STADIUM2
        };
    }

    namespace Abilities
    {
        enum abilities
        {
            NONE,
            STENCH,
            DRIZZLE,
            SPEED_BOOST,
            BATTLE_ARMOR,
            STURDY,
            DAMP,
            LIMBER,
            SAND_VEIL,
            STATIC,
            VOLT_ABSORB,
            WATER_ABSORB,
            OBLIVIOUS,
            CLOUD_NINE,
            COMPOUNDEYES,
            INSOMNIA,
            COLOR_CHANGE,
            IMMUNITY,
            FLASH_FIRE,
            SHIELD_DUST,
            OWN_TEMPO,
            SUCTION_CUPS,
            INTIMIDATE,
            SHADOW_TAG,
            ROUGH_SKIN,
            WONDER_GUARD,
            LEVITATE,
            EFFECT_SPORE,
            SYNCHRONIZE,
            CLEAR_BODY,
            NATURAL_CURE,
            LIGHTNINGROD,
            SERENE_GRACE,
            SWIFT_SWIM,
            CHLOROPHYLL,
            ILLUMINATE,
            TRACE,
            HUGE_POWER,
            POISON_POINT,
            INNER_FOCUS,
            MAGMA_ARMOR,
            WATER_VEIL,
            MAGNET_PULL,
            SOUNDPROOF,
            RAIN_DISH,
            SAND_STREAM,
            PRESSURE,
            THICK_FAT,
            EARLY_BIRD,
            FLAME_BODY,
            RUN_AWAY,
            KEEN_EYE,
            HYPER_CUTTER,
            PICKUP,
            TRUANT,
            HUSTLE,
            CUTE_CHARM,
            PLUS,
            MINUS,
            FORECAST,
            STICKY_HOLD,
            SHED_SKIN,
            GUTS,
            MARVEL_SCALE,
            LIQUID_OOZE,
            OVERGROW,
            BLAZE,
            TORRENT,
            SWARM,
            ROCK_HEAD,
            DROUGHT,
            ARENA_TRAP,
            VITAL_SPIRIT,
            WHITE_SMOKE,
            PURE_POWER,
            SHELL_ARMOR,
            AIR_LOCK,
            TANGLED_FEET,
            MOTOR_DRIVE,
            RIVALRY,
            STEADFAST,
            SNOW_CLOAK,
            GLUTTONY,
            ANGER_POINT,
            UNBURDEN,
            HEATPROOF,
            SIMPLE,
            DRY_SKIN,
            DOWNLOAD,
            IRON_FIST,
            POISON_HEAL,
            ADAPTABILITY,
            SKILL_LINK,
            HYDRATION,
            SOLAR_POWER,
            QUICK_FEET,
            NORMALIZE,
            SNIPER,
            MAGIC_GUARD,
            NO_GUARD,
            STALL,
            TECHNICIAN,
            LEAF_GUARD,
            KLUTZ,
            MOLD_BREAKER,
            SUPER_LUCK,
            AFTERMATH,
            ANTICIPATION,
            FOREWARN,
            UNAWARE,
            TINTED_LENS,
            FILTER,
            SLOW_START,
            SCRAPPY,
            STORM_DRAIN,
            ICE_BODY,
            SOLID_ROCK,
            SNOW_WARNING,
            HONEY_GATHER,
            FRISK,
            RECKLESS,
            MULTITYPE,
            FLOWER_GIFT,
            BAD_DREAMS,
            PICKPOCKET,
            SHEER_FORCE,
            CONTRARY,
            UNNERVE,
            DEFIANT,
            DEFEATIST,
            CURSED_BODY,
            HEALER,
            FRIEND_GUARD,
            WEAK_ARMOR,
            HEAVY_METAL,
            LIGHT_METAL,
            MULTISCALE,
            TOXIC_BOOST,
            FLARE_BOOST,
            HARVEST,
            TELEPATHY,
            MOODY,
            OVERCOAT,
            POISON_TOUCH,
            REGENERATOR,
            BIG_PECKS,
            SAND_RUSH,
            WONDER_SKIN,
            ANALYTIC,
            ILLUSION,
            IMPOSTER,
            INFILTRATOR,
            MUMMY,
            MOXIE,
            JUSTIFIED,
            RATTLED,
            MAGIC_BOUNCE,
            SAP_SIPPER,
            PRANKSTER,
            SAND_FORCE,
            IRON_BARBS,
            ZEN_MODE,
            VICTORY_STAR,
            TURBOBLAZE,
            TERAVOLT,
            MOUNTAINEER,
            WAVE_RIDER,
            SKATER,
            THRUST,
            PERCEPTION,
            PARRY,
            INSTINCT,
            DODGE,
            JAGGED_EDGE,
            FROSTBITE,
            TENACITY,
            PRIDE,
            DEEP_SLEEP,
            POWER_NAP,
            SPIRIT,
            WARM_BLANKET,
            GULP,
            HERBIVORE,
            SANDPIT,
            HOT_BLOODED,
            MEDIC,
            LIFE_FORCE,
            LUNCHBOX,
            NURSE,
            MELEE,
            SPONGE,
            BODYGUARD,
            HERO,
            LAST_BASTION,
            STEALTH,
            VANGUARD,
            NOMAD,
            SEQUENCE,
            GRASS_CLOAK,
            CELEBRATE,
            LULLABY,
            CALMING,
            DAZE,
            FRIGHTEN,
            INTERFERENCE,
            MOOD_MAKER,
            CONFIDENCE,
            FORTUNE,
            BONANZA,
            EXPLODE,
            OMNIPOTENT,
            SHARE,
            BLACK_HOLE,
            SHADOW_DASH,
            SPRINT,
            DISGUST,
            HIGH_RISE,
            CLIMBER,
            FLAME_BOOST,
            AQUA_BOOST,
            RUN_UP,
            CONQUEROR,
            SHACKLE,
            DECOY,
            SHIELD
        };
    }

    namespace Moves
    {
        enum moves
        {
            NONE,
            POUND,
            KARATE_CHOP,
            DOUBLESLAP,
            COMET_PUNCH,
            MEGA_PUNCH,
            PAY_DAY,
            FIRE_PUNCH,
            ICE_PUNCH,
            THUNDERPUNCH,
            SCRATCH,
            VICEGRIP,
            GUILLOTINE,
            RAZOR_WIND,
            SWORDS_DANCE,
            CUT,
            GUST,
            WING_ATTACK,
            WHIRLWIND,
            FLY,
            BIND,
            SLAM,
            VINE_WHIP,
            STOMP,
            DOUBLE_KICK,
            MEGA_KICK,
            JUMP_KICK,
            ROLLING_KICK,
            SAND_ATTACK,
            HEADBUTT,
            HORN_ATTACK,
            FURY_ATTACK,
            HORN_DRILL,
            TACKLE,
            BODY_SLAM,
            WRAP,
            TAKE_DOWN,
            THRASH,
            DOUBLE_EDGE,
            TAIL_WHIP,
            POISON_STING,
            TWINEEDLE,
            PIN_MISSILE,
            LEER,
            BITE,
            GROWL,
            ROAR,
            __SING,
            SUPERSONIC,
            SONICBOOM,
            DISABLE,
            ACID,
            EMBER,
            FLAMETHROWER,
            MIST,
            WATER_GUN,
            HYDRO_PUMP,
            SURF,
            ICE_BEAM,
            BLIZZARD,
            PSYBEAM,
            BUBBLEBEAM,
            AURORA_BEAM,
            HYPER_BEAM,
            PECK,
            DRILL_PECK,
            SUBMISSION,
            LOW_KICK,
            COUNTER,
            SEISMIC_TOSS,
            STRENGTH,
            ABSORB,
            MEGA_DRAIN,
            LEECH_SEED,
            GROWTH,
            RAZOR_LEAF,
            SOLARBEAM,
            POISONPOWDER,
            STUN_SPORE,
            SLEEP_POWDER,
            PETAL_DANCE,
            STRING_SHOT,
            DRAGON_RAGE,
            FIRE_SPIN,
            THUNDERSHOCK,
            THUNDERBOLT,
            THUNDER_WAVE,
            THUNDER,
            ROCK_THROW,
            EARTHQUAKE,
            FISSURE,
            DIG,
            TOXIC,
            CONFUSION,
            PSYCHIC,
            HYPNOSIS,
            MEDITATE,
            AGILITY,
            QUICK_ATTACK,
            RAGE,
            TELEPORT,
            NIGHT_SHADE,
            MIMIC,
            SCREECH,
            DOUBLE_TEAM,
            RECOVER,
            HARDEN,
            MINIMIZE,
            SMOKESCREEN,
            CONFUSE_RAY,
            WITHDRAW,
            DEFENSE_CURL,
            BARRIER,
            LIGHT_SCREEN,
            HAZE,
            REFLECT,
            FOCUS_ENERGY,
            BIDE,
            METRONOME,
            MIRROR_MOVE,
            SELFDESTRUCT,
            EGG_BOMB,
            LICK,
            SMOG,
            SLUDGE,
            BONE_CLUB,
            FIRE_BLAST,
            WATERFALL,
            CLAMP,
            SWIFT,
            SKULL_BASH,
            SPIKE_CANNON,
            CONSTRICT,
            AMNESIA,
            KINESIS,
            SOFTBOILED,
            HI_JUMP_KICK,
            GLARE,
            DREAM_EATER,
            POISON_GAS,
            BARRAGE,
            LEECH_LIFE,
            LOVELY_KISS,
            SKY_ATTACK,
            TRANSFORM,
            BUBBLE,
            DIZZY_PUNCH,
            SPORE,
            FLASH,
            PSYWAVE,
            SPLASH,
            ACID_ARMOR,
            CRABHAMMER,
            EXPLOSION,
            FURY_SWIPES,
            BONEMERANG,
            REST,
            ROCK_SLIDE,
            HYPER_FANG,
            SHARPEN,
            CONVERSION,
            TRI_ATTACK,
            SUPER_FANG,
            SLASH,
            SUBSTITUTE,
            STRUGGLE,
            SKETCH,
            TRIPLE_KICK,
            THIEF,
            SPIDER_WEB,
            MIND_READER,
            NIGHTMARE,
            FLAME_WHEEL,
            SNORE,
            CURSE,
            FLAIL,
            CONVERSION_2,
            AEROBLAST,
            COTTON_SPORE,
            REVERSAL,
            SPITE,
            POWDER_SNOW,
            PROTECT,
            MACH_PUNCH,
            SCARY_FACE,
            FAINT_ATTACK,
            SWEET_KISS,
            BELLY_DRUM,
            SLUDGE_BOMB,
            MUD_SLAP,
            OCTAZOOKA,
            SPIKES,
            ZAP_CANNON,
            FORESIGHT,
            DESTINY_BOND,
            PERISH_SONG,
            ICY_WIND,
            DETECT,
            BONE_RUSH,
            LOCK_ON,
            OUTRAGE,
            SANDSTORM,
            GIGA_DRAIN,
            ENDURE,
            CHARM,
            ROLLOUT,
            FALSE_SWIPE,
            SWAGGER,
            MILK_DRINK,
            SPARK,
            FURY_CUTTER,
            STEEL_WING,
            MEAN_LOOK,
            ATTRACT,
            SLEEP_TALK,
            HEAL_BELL,
            RETURN,
            PRESENT,
            FRUSTRATION,
            SAFEGUARD,
            PAIN_SPLIT,
            SACRED_FIRE,
            MAGNITUDE,
            DYNAMICPUNCH,
            MEGAHORN,
            DRAGONBREATH,
            BATON_PASS,
            ENCORE,
            PURSUIT,
            RAPID_SPIN,
            SWEET_SCENT,
            IRON_TAIL,
            METAL_CLAW,
            VITAL_THROW,
            MORNING_SUN,
            SYNTHESIS,
            MOONLIGHT,
            HIDDEN_POWER,
            CROSS_CHOP,
            TWISTER,
            RAIN_DANCE,
            SUNNY_DAY,
            CRUNCH,
            MIRROR_COAT,
            PSYCH_UP,
            EXTREMESPEED,
            ANCIENTPOWER,
            SHADOW_BALL,
            FUTURE_SIGHT,
            ROCK_SMASH,
            WHIRLPOOL,
            BEAT_UP,
            FAKE_OUT,
            UPROAR,
            STOCKPILE,
            SPIT_UP,
            SWALLOW,
            HEAT_WAVE,
            HAIL,
            TORMENT,
            FLATTER,
            WILL_O_WISP,
            MEMENTO,
            FACADE,
            FOCUS_PUNCH,
            SMELLINGSALT,
            FOLLOW_ME,
            NATURE_POWER,
            CHARGE,
            TAUNT,
            HELPING_HAND,
            TRICK,
            ROLE_PLAY,
            WISH,
            ASSIST,
            INGRAIN,
            SUPERPOWER,
            MAGIC_COAT,
            RECYCLE,
            REVENGE,
            BRICK_BREAK,
            YAWN,
            KNOCK_OFF,
            ENDEAVOR,
            ERUPTION,
            SKILL_SWAP,
            IMPRISON,
            REFRESH,
            GRUDGE,
            SNATCH,
            SECRET_POWER,
            DIVE,
            ARM_THRUST,
            CAMOUFLAGE,
            TAIL_GLOW,
            LUSTER_PURGE,
            MIST_BALL,
            FEATHERDANCE,
            TEETER_DANCE,
            BLAZE_KICK,
            MUD_SPORT,
            ICE_BALL,
            NEEDLE_ARM,
            SLACK_OFF,
            HYPER_VOICE,
            POISON_FANG,
            CRUSH_CLAW,
            BLAST_BURN,
            HYDRO_CANNON,
            METEOR_MASH,
            ASTONISH,
            WEATHER_BALL,
            AROMATHERAPY,
            FAKE_TEARS,
            AIR_CUTTER,
            OVERHEAT,
            ODOR_SLEUTH,
            ROCK_TOMB,
            SILVER_WIND,
            METAL_SOUND,
            GRASSWHISTLE,
            TICKLE,
            COSMIC_POWER,
            WATER_SPOUT,
            SIGNAL_BEAM,
            SHADOW_PUNCH,
            EXTRASENSORY,
            SKY_UPPERCUT,
            SAND_TOMB,
            SHEER_COLD,
            MUDDY_WATER,
            BULLET_SEED,
            AERIAL_ACE,
            ICICLE_SPEAR,
            IRON_DEFENSE,
            BLOCK,
            HOWL,
            DRAGON_CLAW,
            FRENZY_PLANT,
            BULK_UP,
            BOUNCE,
            MUD_SHOT,
            POISON_TAIL,
            COVET,
            VOLT_TACKLE,
            MAGICAL_LEAF,
            WATER_SPORT,
            CALM_MIND,
            LEAF_BLADE,
            DRAGON_DANCE,
            ROCK_BLAST,
            SHOCK_WAVE,
            WATER_PULSE,
            DOOM_DESIRE,
            PSYCHO_BOOST,
            ROOST,
            GRAVITY,
            MIRACLE_EYE,
            WAKE_UP_SLAP,
            HAMMER_ARM,
            GYRO_BALL,
            HEALING_WISH,
            BRINE,
            NATURAL_GIFT,
            FEINT,
            PLUCK,
            TAILWIND,
            ACUPRESSURE,
            METAL_BURST,
            U_TURN,
            CLOSE_COMBAT,
            PAYBACK,
            ASSURANCE,
            EMBARGO,
            FLING,
            PSYCHO_SHIFT,
            TRUMP_CARD,
            HEAL_BLOCK,
            WRING_OUT,
            POWER_TRICK,
            GASTRO_ACID,
            LUCKY_CHANT,
            ME_FIRST,
            COPYCAT,
            POWER_SWAP,
            GUARD_SWAP,
            PUNISHMENT,
            LAST_RESORT,
            WORRY_SEED,
            SUCKER_PUNCH,
            TOXIC_SPIKES,
            HEART_SWAP,
            AQUA_RING,
            MAGNET_RISE,
            FLARE_BLITZ,
            FORCE_PALM,
            AURA_SPHERE,
            ROCK_POLISH,
            POISON_JAB,
            DARK_PULSE,
            NIGHT_SLASH,
            AQUA_TAIL,
            SEED_BOMB,
            AIR_SLASH,
            X_SCISSOR,
            BUG_BUZZ,
            DRAGON_PULSE,
            DRAGON_RUSH,
            POWER_GEM,
            DRAIN_PUNCH,
            VACUUM_WAVE,
            FOCUS_BLAST,
            ENERGY_BALL,
            BRAVE_BIRD,
            EARTH_POWER,
            SWITCHEROO,
            GIGA_IMPACT,
            NASTY_PLOT,
            BULLET_PUNCH,
            AVALANCHE,
            ICE_SHARD,
            SHADOW_CLAW,
            THUNDER_FANG,
            ICE_FANG,
            FIRE_FANG,
            SHADOW_SNEAK,
            MUD_BOMB,
            PSYCHO_CUT,
            ZEN_HEADBUTT,
            MIRROR_SHOT,
            FLASH_CANNON,
            ROCK_CLIMB,
            DEFOG,
            TRICK_ROOM,
            DRACO_METEOR,
            DISCHARGE,
            LAVA_PLUME,
            LEAF_STORM,
            POWER_WHIP,
            ROCK_WRECKER,
            CROSS_POISON,
            GUNK_SHOT,
            IRON_HEAD,
            MAGNET_BOMB,
            STONE_EDGE,
            CAPTIVATE,
            STEALTH_ROCK,
            GRASS_KNOT,
            CHATTER,
            JUDGMENT,
            BUG_BITE,
            CHARGE_BEAM,
            WOOD_HAMMER,
            AQUA_JET,
            ATTACK_ORDER,
            DEFEND_ORDER,
            HEAL_ORDER,
            HEAD_SMASH,
            DOUBLE_HIT,
            ROAR_OF_TIME,
            SPACIAL_REND,
            LUNAR_DANCE,
            CRUSH_GRIP,
            MAGMA_STORM,
            DARK_VOID,
            SEED_FLARE,
            OMINOUS_WIND,
            SHADOW_FORCE,
            HONE_CLAWS,
            WIDE_GUARD,
            GUARD_SPLIT,
            POWER_SPLIT,
            WONDER_ROOM,
            PSYSHOCK,
            VENOSHOCK,
            AUTOTOMIZE,
            RAGE_POWDER,
            TELEKINESIS,
            MAGIC_ROOM,
            SMACK_DOWN,
            STORM_THROW,
            FLAME_BURST,
            SLUDGE_WAVE,
            QUIVER_DANCE,
            HEAVY_SLAM,
            SYNCHRONOISE,
            ELECTRO_BALL,
            SOAK,
            FLAME_CHARGE,
            COIL,
            LOW_SWEEP,
            ACID_SPRAY,
            FOUL_PLAY,
            SIMPLE_BEAM,
            ENTRAINMENT,
            AFTER_YOU,
            ROUND,
            ECHOED_VOICE,
            CHIP_AWAY,
            CLEAR_SMOG,
            STORED_POWER,
            QUICK_GUARD,
            ALLY_SWITCH,
            SCALD,
            SHELL_SMASH,
            HEAL_PULSE,
            HEX,
            SKY_DROP,
            SHIFT_GEAR,
            CIRCLE_THROW,
            INCINERATE,
            QUASH,
            ACROBATICS,
            REFLECT_TYPE,
            RETALIATE,
            FINAL_GAMBIT,
            BESTOW,
            INFERNO,
            WATER_PLEDGE,
            FIRE_PLEDGE,
            GRASS_PLEDGE,
            VOLT_SWITCH,
            STRUGGLE_BUG,
            BULLDOZE,
            FROST_BREATH,
            DRAGON_TAIL,
            WORK_UP,
            ELECTROWEB,
            WILD_CHARGE,
            DRILL_RUN,
            DUAL_CHOP,
            HEART_STAMP,
            HORN_LEECH,
            SACRED_SWORD,
            RAZOR_SHELL,
            HEAT_CRASH,
            LEAF_TORNADO,
            STEAMROLLER,
            COTTON_GUARD,
            NIGHT_DAZE,
            PSYSTRIKE,
            TAIL_SLAP,
            HURRICANE,
            HEAD_CHARGE,
            GEAR_GRIND,
            SEARING_SHOT,
            TECHNO_BLAST,
            RELIC_SONG,
            SECRET_SWORD,
            GLACIATE,
            BOLT_STRIKE,
            BLUE_FLARE,
            FIERY_DANCE,
            FREEZE_SHOCK,
            ICE_BURN,
            SNARL,
            ICICLE_CRASH,
            V_CREATE,
            FUSION_FLARE,
            FUSION_BOLT
        };
    }

    namespace Natures
    {
        enum natures
        {
            NONE,
            HARDY,
            BOLD,
            MODEST,
            CALM,
            TIMID,
            LONELY,
            DOCILE,
            MILD,
            GENTLE,
            HASTY,
            ADAMANT,
            IMPISH,
            BASHFUL,
            CAREFUL,
            RASH,
            JOLLY,
            NAUGHTY,
            LAX,
            QUIRKY,
            NAIVE,
            BRAVE,
            RELAXED,
            QUIET,
            SASSY,
            SERIOUS
        };
    }

    namespace Species
    {
        enum species
        {
            NONE,
            BULBASAUR,
            IVYSAUR,
            VENUSAUR,
            CHARMANDER,
            CHARMELEON,
            CHARIZARD,
            SQUIRTLE,
            WARTORTLE,
            BLASTOISE,
            CATERPIE,
            METAPOD,
            BUTTERFREE,
            WEEDLE,
            KAKUNA,
            BEEDRILL,
            PIDGEY,
            PIDGEOTTO,
            PIDGEOT,
            RATTATA,
            RATICATE,
            SPEAROW,
            FEAROW,
            EKANS,
            ARBOK,
            PIKACHU,
            RAICHU,
            SANDSHREW,
            SANDSLASH,
            NIDORAN_F,
            NIDORINA,
            NIDOQUEEN,
            NIDORAN_M,
            NIDORINO,
            NIDOKING,
            CLEFAIRY,
            CLEFABLE,
            VULPIX,
            NINETALES,
            JIGGLYPUFF,
            WIGGLYTUFF,
            ZUBAT,
            GOLBAT,
            ODDISH,
            GLOOM,
            VILEPLUME,
            PARAS,
            PARASECT,
            VENONAT,
            VENOMOTH,
            DIGLETT,
            DUGTRIO,
            MEOWTH,
            PERSIAN,
            PSYDUCK,
            GOLDUCK,
            MANKEY,
            PRIMEAPE,
            GROWLITHE,
            ARCANINE,
            POLIWAG,
            POLIWHIRL,
            POLIWRATH,
            ABRA,
            KADABRA,
            ALAKAZAM,
            MACHOP,
            MACHOKE,
            MACHAMP,
            BELLSPROUT,
            WEEPINBELL,
            VICTREEBEL,
            TENTACOOL,
            TENTACRUEL,
            GEODUDE,
            GRAVELER,
            GOLEM,
            PONYTA,
            RAPIDASH,
            SLOWPOKE,
            SLOWBRO,
            MAGNEMITE,
            MAGNETON,
            FARFETCHD,
            DODUO,
            DODRIO,
            SEEL,
            DEWGONG,
            GRIMER,
            MUK,
            SHELLDER,
            CLOYSTER,
            GASTLY,
            HAUNTER,
            GENGAR,
            ONIX,
            DROWZEE,
            HYPNO,
            KRABBY,
            KINGLER,
            VOLTORB,
            ELECTRODE,
            EXEGGCUTE,
            EXEGGUTOR,
            CUBONE,
            MAROWAK,
            HITMONLEE,
            HITMONCHAN,
            LICKITUNG,
            KOFFING,
            WEEZING,
            RHYHORN,
            RHYDON,
            CHANSEY,
            TANGELA,
            KANGASKHAN,
            HORSEA,
            SEADRA,
            GOLDEEN,
            SEAKING,
            STARYU,
            STARMIE,
            MR_MIME,
            SCYTHER,
            JYNX,
            ELECTABUZZ,
            MAGMAR,
            PINSIR,
            TAUROS,
            MAGIKARP,
            GYARADOS,
            LAPRAS,
            DITTO,
            EEVEE,
            VAPOREON,
            JOLTEON,
            FLAREON,
            PORYGON,
            OMANYTE,
            OMASTAR,
            KABUTO,
            KABUTOPS,
            AERODACTYL,
            SNORLAX,
            ARTICUNO,
            ZAPDOS,
            MOLTRES,
            DRATINI,
            DRAGONAIR,
            DRAGONITE,
            MEWTWO,
            MEW,
            CHIKORITA,
            BAYLEEF,
            MEGANIUM,
            CYNDAQUIL,
            QUILAVA,
            TYPHLOSION,
            TOTODILE,
            CROCONAW,
            FERALIGATR,
            SENTRET,
            FURRET,
            HOOTHOOT,
            NOCTOWL,
            LEDYBA,
            LEDIAN,
            SPINARAK,
            ARIADOS,
            CROBAT,
            CHINCHOU,
            LANTURN,
            PICHU,
            CLEFFA,
            IGGLYBUFF,
            TOGEPI,
            TOGETIC,
            NATU,
            XATU,
            MAREEP,
            FLAAFFY,
            AMPHAROS,
            BELLOSSOM,
            MARILL,
            AZUMARILL,
            SUDOWOODO,
            POLITOED,
            HOPPIP,
            SKIPLOOM,
            JUMPLUFF,
            AIPOM,
            SUNKERN,
            SUNFLORA,
            YANMA,
            WOOPER,
            QUAGSIRE,
            ESPEON,
            UMBREON,
            MURKROW,
            SLOWKING,
            MISDREAVUS,
            UNOWN,
            WOBBUFFET,
            GIRAFARIG,
            PINECO,
            FORRETRESS,
            DUNSPARCE,
            GLIGAR,
            STEELIX,
            SNUBBULL,
            GRANBULL,
            QWILFISH,
            SCIZOR,
            SHUCKLE,
            HERACROSS,
            SNEASEL,
            TEDDIURSA,
            URSARING,
            SLUGMA,
            MAGCARGO,
            SWINUB,
            PILOSWINE,
            CORSOLA,
            REMORAID,
            OCTILLERY,
            DELIBIRD,
            MANTINE,
            SKARMORY,
            HOUNDOUR,
            HOUNDOOM,
            KINGDRA,
            PHANPY,
            DONPHAN,
            PORYGON2,
            STANTLER,
            SMEARGLE,
            TYROGUE,
            HITMONTOP,
            SMOOCHUM,
            ELEKID,
            MAGBY,
            MILTANK,
            BLISSEY,
            RAIKOU,
            ENTEI,
            SUICUNE,
            LARVITAR,
            PUPITAR,
            TYRANITAR,
            LUGIA,
            HO_OH,
            CELEBI,
            TREECKO,
            GROVYLE,
            SCEPTILE,
            TORCHIC,
            COMBUSKEN,
            BLAZIKEN,
            MUDKIP,
            MARSHTOMP,
            SWAMPERT,
            POOCHYENA,
            MIGHTYENA,
            ZIGZAGOON,
            LINOONE,
            WURMPLE,
            SILCOON,
            BEAUTIFLY,
            CASCOON,
            DUSTOX,
            LOTAD,
            LOMBRE,
            LUDICOLO,
            SEEDOT,
            NUZLEAF,
            SHIFTRY,
            TAILLOW,
            SWELLOW,
            WINGULL,
            PELIPPER,
            RALTS,
            KIRLIA,
            GARDEVOIR,
            SURSKIT,
            MASQUERAIN,
            SHROOMISH,
            BRELOOM,
            SLAKOTH,
            VIGOROTH,
            SLAKING,
            NINCADA,
            NINJASK,
            SHEDINJA,
            WHISMUR,
            LOUDRED,
            EXPLOUD,
            MAKUHITA,
            HARIYAMA,
            AZURILL,
            NOSEPASS,
            SKITTY,
            DELCATTY,
            SABLEYE,
            MAWILE,
            ARON,
            LAIRON,
            AGGRON,
            MEDITITE,
            MEDICHAM,
            ELECTRIKE,
            MANECTRIC,
            PLUSLE,
            MINUN,
            VOLBEAT,
            ILLUMISE,
            ROSELIA,
            GULPIN,
            SWALOT,
            CARVANHA,
            SHARPEDO,
            WAILMER,
            WAILORD,
            NUMEL,
            CAMERUPT,
            TORKOAL,
            SPOINK,
            GRUMPIG,
            SPINDA,
            TRAPINCH,
            VIBRAVA,
            FLYGON,
            CACNEA,
            CACTURNE,
            SWABLU,
            ALTARIA,
            ZANGOOSE,
            SEVIPER,
            LUNATONE,
            SOLROCK,
            BARBOACH,
            WHISCASH,
            CORPHISH,
            CRAWDAUNT,
            BALTOY,
            CLAYDOL,
            LILEEP,
            CRADILY,
            ANORITH,
            ARMALDO,
            FEEBAS,
            MILOTIC,
            CASTFORM,
            KECLEON,
            SHUPPET,
            BANETTE,
            DUSKULL,
            DUSCLOPS,
            TROPIUS,
            CHIMECHO,
            ABSOL,
            WYNAUT,
            SNORUNT,
            GLALIE,
            SPHEAL,
            SEALEO,
            WALREIN,
            CLAMPERL,
            HUNTAIL,
            GOREBYSS,
            RELICANTH,
            LUVDISC,
            BAGON,
            SHELGON,
            SALAMENCE,
            BELDUM,
            METANG,
            METAGROSS,
            REGIROCK,
            REGICE,
            REGISTEEL,
            LATIAS,
            LATIOS,
            KYOGRE,
            GROUDON,
            RAYQUAZA,
            JIRACHI,
            DEOXYS,
            TURTWIG,
            GROTLE,
            TORTERRA,
            CHIMCHAR,
            MONFERNO,
            INFERNAPE,
            PIPLUP,
            PRINPLUP,
            EMPOLEON,
            STARLY,
            STARAVIA,
            STARAPTOR,
            BIDOOF,
            BIBAREL,
            KRICKETOT,
            KRICKETUNE,
            SHINX,
            LUXIO,
            LUXRAY,
            BUDEW,
            ROSERADE,
            CRANIDOS,
            RAMPARDOS,
            SHIELDON,
            BASTIODON,
            BURMY,
            WORMADAM,
            MOTHIM,
            COMBEE,
            VESPIQUEN,
            PACHIRISU,
            BUIZEL,
            FLOATZEL,
            CHERUBI,
            CHERRIM,
            SHELLOS,
            GASTRODON,
            AMBIPOM,
            DRIFLOON,
            DRIFBLIM,
            BUNEARY,
            LOPUNNY,
            MISMAGIUS,
            HONCHKROW,
            GLAMEOW,
            PURUGLY,
            CHINGLING,
            STUNKY,
            SKUNTANK,
            BRONZOR,
            BRONZONG,
            BONSLY,
            MIME_JR,
            HAPPINY,
            CHATOT,
            SPIRITOMB,
            GIBLE,
            GABITE,
            GARCHOMP,
            MUNCHLAX,
            RIOLU,
            LUCARIO,
            HIPPOPOTAS,
            HIPPOWDON,
            SKORUPI,
            DRAPION,
            CROAGUNK,
            TOXICROAK,
            CARNIVINE,
            FINNEON,
            LUMINEON,
            MANTYKE,
            SNOVER,
            ABOMASNOW,
            WEAVILE,
            MAGNEZONE,
            LICKILICKY,
            RHYPERIOR,
            TANGROWTH,
            ELECTIVIRE,
            MAGMORTAR,
            TOGEKISS,
            YANMEGA,
            LEAFEON,
            GLACEON,
            GLISCOR,
            MAMOSWINE,
            PORYGON_Z,
            GALLADE,
            PROBOPASS,
            DUSKNOIR,
            FROSLASS,
            ROTOM,
            UXIE,
            MESPRIT,
            AZELF,
            DIALGA,
            PALKIA,
            HEATRAN,
            REGIGIGAS,
            GIRATINA,
            CRESSELIA,
            PHIONE,
            MANAPHY,
            DARKRAI,
            SHAYMIN,
            ARCEUS,
            VICTINI,
            SNIVY,
            SERVINE,
            SERPERIOR,
            TEPIG,
            PIGNITE,
            EMBOAR,
            OSHAWOTT,
            DEWOTT,
            SAMUROTT,
            PATRAT,
            WATCHOG,
            LILLIPUP,
            HERDIER,
            STOUTLAND,
            PURRLOIN,
            LIEPARD,
            PANSAGE,
            SIMISAGE,
            PANSEAR,
            SIMISEAR,
            PANPOUR,
            SIMIPOUR,
            MUNNA,
            MUSHARNA,
            PIDOVE,
            TRANQUILL,
            UNFEZANT,
            BLITZLE,
            ZEBSTRIKA,
            ROGGENROLA,
            BOLDORE,
            GIGALITH,
            WOOBAT,
            SWOOBAT,
            DRILBUR,
            EXCADRILL,
            AUDINO,
            TIMBURR,
            GURDURR,
            CONKELDURR,
            TYMPOLE,
            PALPITOAD,
            SEISMITOAD,
            THROH,
            SAWK,
            SEWADDLE,
            SWADLOON,
            LEAVANNY,
            VENIPEDE,
            WHIRLIPEDE,
            SCOLIPEDE,
            COTTONEE,
            WHIMSICOTT,
            PETILIL,
            LILLIGANT,
            BASCULIN,
            SANDILE,
            KROKOROK,
            KROOKODILE,
            DARUMAKA,
            DARMANITAN,
            MARACTUS,
            DWEBBLE,
            CRUSTLE,
            SCRAGGY,
            SCRAFTY,
            SIGILYPH,
            YAMASK,
            COFAGRIGUS,
            TIRTOUGA,
            CARRACOSTA,
            ARCHEN,
            ARCHEOPS,
            TRUBBISH,
            GARBODOR,
            ZORUA,
            ZOROARK,
            MINCCINO,
            CINCCINO,
            GOTHITA,
            GOTHORITA,
            GOTHITELLE,
            SOLOSIS,
            DUOSION,
            REUNICLUS,
            DUCKLETT,
            SWANNA,
            VANILLITE,
            VANILLISH,
            VANILLUXE,
            DEERLING,
            SAWSBUCK,
            EMOLGA,
            KARRABLAST,
            ESCAVALIER,
            FOONGUS,
            AMOONGUSS,
            FRILLISH,
            JELLICENT,
            ALOMOMOLA,
            JOLTIK,
            GALVANTULA,
            FERROSEED,
            FERROTHORN,
            KLINK,
            KLANG,
            KLINKLANG,
            TYNAMO,
            EELEKTRIK,
            EELEKTROSS,
            ELGYEM,
            BEHEEYEM,
            LITWICK,
            LAMPENT,
            CHANDELURE,
            AXEW,
            FRAXURE,
            HAXORUS,
            CUBCHOO,
            BEARTIC,
            CRYOGONAL,
            SHELMET,
            ACCELGOR,
            STUNFISK,
            MIENFOO,
            MIENSHAO,
            DRUDDIGON,
            GOLETT,
            GOLURK,
            PAWNIARD,
            BISHARP,
            BOUFFALANT,
            RUFFLET,
            BRAVIARY,
            VULLABY,
            MANDIBUZZ,
            HEATMOR,
            DURANT,
            DEINO,
            ZWEILOUS,
            HYDREIGON,
            LARVESTA,
            VOLCARONA,
            COBALION,
            TERRAKION,
            VIRIZION,
            TORNADUS,
            THUNDURUS,
            RESHIRAM,
            ZEKROM,
            LANDORUS,
            KYUREM,
            KELDEO,
            MELOETTA,
            GENESECT
        };
    }
}

#endif /* INCLUDED_PKMNSIM_ENUMS_HPP */
