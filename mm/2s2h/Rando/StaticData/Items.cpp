#include "StaticData.h"
#include "libultraship/bridge.h"

extern "C" {
extern s16 D_801CFF94[250];
#include "assets/interface/parameter_static/parameter_static.h"
}

namespace Rando {

namespace StaticData {

#define RI(id, article, name, type, itemId, getItemId, drawId)      \
    {                                                               \
        id, {                                                       \
            id, #id, article, name, type, itemId, getItemId, drawId \
        }                                                           \
    }

// clang-format off
std::map<RandoItemId, RandoStaticItem> Items = {
    RI(RI_UNKNOWN,                    "",     "Unknown",                    RITYPE_JUNK,            ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_ARROW_FIRE,                 "",     "Fire Arrows",                RITYPE_MAJOR,           ITEM_ARROW_FIRE,                 GI_ARROW_FIRE,               GID_ARROW_FIRE),
    RI(RI_ARROW_ICE,                  "",     "Ice Arrows",                 RITYPE_MAJOR,           ITEM_ARROW_ICE,                  GI_ARROW_ICE,                GID_ARROW_ICE),
    RI(RI_ARROW_LIGHT,                "",     "Light Arrows",               RITYPE_MAJOR,           ITEM_ARROW_LIGHT,                GI_ARROW_LIGHT,              GID_ARROW_LIGHT),
    RI(RI_ARROWS_10,                  "",     "10 Arrows",                  RITYPE_JUNK,            ITEM_ARROWS_10,                  GI_ARROWS_10,                GID_ARROWS_SMALL),
    RI(RI_ARROWS_30,                  "",     "30 Arrows",                  RITYPE_JUNK,            ITEM_ARROWS_30,                  GI_ARROWS_30,                GID_ARROWS_MEDIUM),
    RI(RI_ARROWS_50,                  "",     "50 Arrows",                  RITYPE_JUNK,            ITEM_ARROWS_50,                  GI_ARROWS_50,                GID_ARROWS_LARGE),
    RI(RI_BLUE_POTION_REFILL,         "a",    "Blue Potion Refill",         RITYPE_JUNK,            ITEM_POTION_BLUE,                GI_POTION_BLUE,              GID_POTION_BLUE),
    RI(RI_BOMB_BAG_20,                "a",    "Bomb Bag",                   RITYPE_MAJOR,           ITEM_BOMB_BAG_20,                GI_BOMB_BAG_20,              GID_BOMB_BAG_20),
    RI(RI_BOMB_BAG_30,                "a",    "Big Bomb Bag",               RITYPE_LESSER,          ITEM_BOMB_BAG_30,                GI_BOMB_BAG_30,              GID_BOMB_BAG_30),
    RI(RI_BOMB_BAG_40,                "the",  "Biggest Bomb Bag",           RITYPE_LESSER,          ITEM_BOMB_BAG_40,                GI_BOMB_BAG_40,              GID_BOMB_BAG_40),
    RI(RI_BOMBCHU_10,                 "",     "10 Bombchus",                RITYPE_JUNK,            ITEM_BOMBCHUS_10,                GI_BOMBCHUS_10,              GID_BOMBCHU),
    RI(RI_BOMBCHU_5,                  "",     "5 Bombchus",                 RITYPE_JUNK,            ITEM_BOMBCHUS_5,                 GI_BOMBCHUS_5,               GID_BOMBCHU),
    RI(RI_BOMBCHU,                    "a",    "Bombchu",                    RITYPE_JUNK,            ITEM_BOMBCHUS_1,                 GI_BOMBCHUS_1,               GID_BOMBCHU), // not sure about this
    RI(RI_BOMBERS_NOTEBOOK,           "the",  "Bomber's Notebook",          RITYPE_LESSER,          ITEM_BOMBERS_NOTEBOOK,           GI_BOMBERS_NOTEBOOK,         GID_BOMBERS_NOTEBOOK),
    RI(RI_BOMBS_10,                   "",     "10 Bombs",                   RITYPE_JUNK,            ITEM_BOMBS_10,                   GI_BOMBS_10,                 GID_BOMB),
    RI(RI_BOMBS_5,                    "",     "5 Bombs",                    RITYPE_JUNK,            ITEM_BOMBS_5,                    GI_BOMBS_5,                  GID_BOMB),
    RI(RI_BOTTLE_CHATEAU_ROMANI,      "a",    "Bottle of Chateau Romani",   RITYPE_MAJOR,           ITEM_CHATEAU,                    GI_CHATEAU,                  GID_CHATEAU),
    RI(RI_BOTTLE_EMPTY,               "an",   "Empty Bottle",               RITYPE_MAJOR,           ITEM_BOTTLE,                     GI_BOTTLE,                   GID_BOTTLE),
    RI(RI_BOTTLE_GOLD_DUST,           "a",    "Bottle With Gold Dust",      RITYPE_MAJOR,           ITEM_GOLD_DUST,                  GI_GOLD_DUST,                GID_GOLD_DUST),
    RI(RI_BOTTLE_MILK,                "a",    "Bottle of Milk",             RITYPE_MAJOR,           ITEM_MILK_BOTTLE,                GI_MILK_BOTTLE,              GID_MILK),
    RI(RI_BOTTLE_RED_POTION,          "a",    "Bottle with Red Potion",     RITYPE_MAJOR,           ITEM_POTION_RED,                 GI_POTION_RED_BOTTLE,        GID_POTION_RED),
    RI(RI_BOW,                        "a",    "Bow",                        RITYPE_MAJOR,           ITEM_BOW,                        GI_QUIVER_30,                GID_BOW),
    RI(RI_CHATEAU_ROMANI_REFILL,      "a",    "Chateau Romani Refill",      RITYPE_JUNK,            ITEM_CHATEAU_2,                  GI_CHATEAU,                  GID_CHATEAU),
    RI(RI_CLOCK_TOWN_STRAY_FAIRY,     "a",    "Clock Town Stray Fairy",     RITYPE_STRAY_FAIRY,     ITEM_STRAY_FAIRIES,              GI_STRAY_FAIRY,              GID_NONE),
    RI(RI_DEED_LAND,                  "the",  "Land Title Deed",            RITYPE_MAJOR,           ITEM_DEED_LAND,                  GI_DEED_LAND,                GID_DEED_LAND),
    RI(RI_DEED_MOUNTAIN,              "the",  "Mountain Title Deed",        RITYPE_MAJOR,           ITEM_DEED_MOUNTAIN,              GI_DEED_MOUNTAIN,            GID_DEED_MOUNTAIN),
    RI(RI_DEED_OCEAN,                 "the",  "Ocean Title Deed",           RITYPE_MAJOR,           ITEM_DEED_OCEAN,                 GI_DEED_OCEAN,               GID_DEED_OCEAN),
    RI(RI_DEED_SWAMP,                 "the",  "Swamp Title Deed",           RITYPE_MAJOR,           ITEM_DEED_SWAMP,                 GI_DEED_SWAMP,               GID_DEED_SWAMP),
    RI(RI_DEKU_NUT,                   "a",    "Deku Nut",                   RITYPE_JUNK,            ITEM_DEKU_NUT,                   GI_DEKU_NUTS_1,              GID_DEKU_NUTS),
    RI(RI_DEKU_NUTS_10,               "",     "10 Deku Nuts",               RITYPE_JUNK,            ITEM_DEKU_NUTS_10,               GI_DEKU_NUTS_10,             GID_DEKU_NUTS),
    RI(RI_DEKU_NUTS_5,                "",     "5 Deku Nuts",                RITYPE_JUNK,            ITEM_DEKU_NUTS_5,                GI_DEKU_NUTS_5,              GID_DEKU_NUTS),
    RI(RI_DEKU_STICK,                 "a",    "Deku Stick",                 RITYPE_JUNK,            ITEM_DEKU_STICK,                 GI_DEKU_STICKS_1,            GID_DEKU_STICK),
    RI(RI_DEKU_STICKS_5,              "",     "5 Deku Sticks",              RITYPE_JUNK,            ITEM_DEKU_STICKS_5,              GI_NONE,                     GID_DEKU_STICK),
    RI(RI_DOUBLE_DEFENSE,             "",     "Double Defense",             RITYPE_HEALTH,          ITEM_NONE,                       GI_NONE,                     GID_HEART_CONTAINER),
    RI(RI_DOUBLE_MAGIC,               "a",    "Magic Upgrade",              RITYPE_LESSER,          ITEM_NONE,                       GI_NONE,                     GID_MAGIC_JAR_BIG),
    RI(RI_FAIRY_REFILL,               "a",    "Fairy",                      RITYPE_JUNK,            ITEM_FAIRY,                      GI_FAIRY,                    GID_FAIRY_2),
    RI(RI_GOLD_DUST_REFILL,           "a",    "Gold Dust Refill",           RITYPE_LESSER,          ITEM_GOLD_DUST_2,                GI_GOLD_DUST_2,              GID_GOLD_DUST),
    RI(RI_GREAT_BAY_BOSS_KEY,         "the",  "Great Bay Boss Key",         RITYPE_BOSS_KEY,        ITEM_KEY_BOSS,                   GI_KEY_BOSS,                 GID_KEY_BOSS),
    RI(RI_GREAT_BAY_COMPASS,          "the",  "Great Bay Compass",          RITYPE_LESSER,          ITEM_COMPASS,                    GI_COMPASS,                  GID_COMPASS),
    RI(RI_GREAT_BAY_MAP,              "the",  "Great Bay Map",              RITYPE_LESSER,          ITEM_DUNGEON_MAP,                GI_MAP,                      GID_DUNGEON_MAP),
    RI(RI_GREAT_BAY_SMALL_KEY,        "a",    "Great Bay Small Key",        RITYPE_SMALL_KEY,       ITEM_KEY_SMALL,                  GI_KEY_SMALL,                GID_KEY_SMALL),
    RI(RI_GREAT_BAY_STRAY_FAIRY,      "a",    "Great Bay Stray Fairy",      RITYPE_STRAY_FAIRY,     ITEM_STRAY_FAIRIES,              GI_STRAY_FAIRY,              GID_NONE),
    RI(RI_GREAT_FAIRY_SWORD,          "the",  "Great Fairy's Sword",        RITYPE_LESSER,          ITEM_SWORD_GREAT_FAIRY,          GI_SWORD_GREAT_FAIRY,        GID_SWORD_GREAT_FAIRY),
    RI(RI_GREAT_SPIN_ATTACK,          "the",  "Great Spin Attack",          RITYPE_LESSER,          ITEM_NONE,                       GI_NONE,                     GID_SWORD_KOKIRI),
    RI(RI_GREEN_POTION_REFILL,        "a",    "Green Potion Refill",        RITYPE_JUNK,            ITEM_POTION_GREEN,               GI_POTION_GREEN,             GID_POTION_GREEN),
    RI(RI_GS_TOKEN_OCEAN,             "an",   "Ocean Gold Skulltula Token", RITYPE_SKULLTULA_TOKEN, ITEM_SKULL_TOKEN,                GI_SKULL_TOKEN,              GID_SKULL_TOKEN_2),
    RI(RI_GS_TOKEN_SWAMP,             "a",    "Swamp Gold Skulltula Token", RITYPE_SKULLTULA_TOKEN, ITEM_SKULL_TOKEN,                GI_SKULL_TOKEN,              GID_SKULL_TOKEN_2),
    RI(RI_HEART_CONTAINER,            "a",    "Heart Container",            RITYPE_HEALTH,          ITEM_HEART_CONTAINER,            GI_HEART_CONTAINER,          GID_HEART_CONTAINER),
    RI(RI_HEART_PIECE,                "a",    "Heart Piece",                RITYPE_HEALTH,          ITEM_HEART_PIECE,                GI_HEART_PIECE,              GID_HEART_PIECE),
    RI(RI_HOOKSHOT,                   "the",  "Hookshot",                   RITYPE_MAJOR,           ITEM_HOOKSHOT,                   GI_HOOKSHOT,                 GID_HOOKSHOT),
    RI(RI_JUNK,                       "",     "Junk",                       RITYPE_JUNK,            ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_LENS,                       "the",  "Lens of Truth",              RITYPE_MAJOR,           ITEM_LENS_OF_TRUTH,              GI_LENS_OF_TRUTH,            GID_LENS),
    RI(RI_LETTER_TO_KAFEI,            "the",  "Letter to Kafei",            RITYPE_MAJOR,           ITEM_LETTER_TO_KAFEI,            GI_LETTER_TO_KAFEI,          GID_LETTER_TO_KAFEI),
    RI(RI_LETTER_TO_MAMA,             "the",  "Letter to Mama",             RITYPE_MAJOR,           ITEM_LETTER_MAMA,                GI_LETTER_TO_MAMA,           GID_LETTER_MAMA),
    RI(RI_MAGIC_BEAN,                 "a",    "Magic Bean",                 RITYPE_LESSER,          ITEM_MAGIC_BEANS,                GI_MAGIC_BEANS,              GID_MAGIC_BEANS),
    RI(RI_MAGIC_JAR_BIG,              "a",    "Large Magic Refill",         RITYPE_JUNK,            ITEM_MAGIC_JAR_BIG,              GI_MAGIC_JAR_BIG,            GID_MAGIC_JAR_BIG),
    RI(RI_MAGIC_JAR_SMALL,            "a",    "Small Magic Refill",         RITYPE_JUNK,            ITEM_MAGIC_JAR_SMALL,            GI_MAGIC_JAR_SMALL,          GID_MAGIC_JAR_SMALL),
    RI(RI_MASK_ALL_NIGHT,             "the",  "All-Night Mask",             RITYPE_MASK,            ITEM_MASK_ALL_NIGHT,             GI_MASK_ALL_NIGHT,           GID_MASK_ALL_NIGHT),
    RI(RI_MASK_BLAST,                 "the",  "Blast Mask",                 RITYPE_MASK,            ITEM_MASK_BLAST,                 GI_MASK_BLAST,               GID_MASK_BLAST),
    RI(RI_MASK_BREMEN,                "the",  "Bremen Mask",                RITYPE_MASK,            ITEM_MASK_BREMEN,                GI_MASK_BREMEN,              GID_MASK_BREMEN),
    RI(RI_MASK_BUNNY,                 "the",  "Bunny Hood",                 RITYPE_MASK,            ITEM_MASK_BUNNY,                 GI_MASK_BUNNY,               GID_MASK_BUNNY),
    RI(RI_MASK_CAPTAIN,               "the",  "Captain's Hat",              RITYPE_MASK,            ITEM_MASK_CAPTAIN,               GI_MASK_CAPTAIN,             GID_MASK_CAPTAIN),
    RI(RI_MASK_CIRCUS_LEADER,         "the",  "Circus Leader's Mask",       RITYPE_MASK,            ITEM_MASK_CIRCUS_LEADER,         GI_MASK_CIRCUS_LEADER,       GID_MASK_CIRCUS_LEADER),
    RI(RI_MASK_COUPLE,                "the",  "Couples Mask",               RITYPE_MASK,            ITEM_MASK_COUPLE,                GI_MASK_COUPLE,              GID_MASK_COUPLE),
    RI(RI_MASK_DEKU,                  "the",  "Deku Mask",                  RITYPE_MASK,            ITEM_MASK_DEKU,                  GI_MASK_DEKU,                GID_MASK_DEKU),
    RI(RI_MASK_DON_GERO,              "the",  "Don Gero Mask",              RITYPE_MASK,            ITEM_MASK_DON_GERO,              GI_MASK_DON_GERO,            GID_MASK_DON_GERO),
    RI(RI_MASK_FIERCE_DEITY,          "the",  "Fierce Deity Mask",          RITYPE_MASK,            ITEM_MASK_FIERCE_DEITY,          GI_MASK_FIERCE_DEITY,        GID_MASK_FIERCE_DEITY),
    RI(RI_MASK_GARO,                  "",     "Garo's Mask",                RITYPE_MASK,            ITEM_MASK_GARO,                  GI_MASK_GARO,                GID_MASK_GARO),
    RI(RI_MASK_GIANT,                 "the",  "Giant's Mask",               RITYPE_MASK,            ITEM_MASK_GIANT,                 GI_MASK_GIANT,               GID_MASK_GIANT),
    RI(RI_MASK_GIBDO,                 "the",  "Gibdo Mask",                 RITYPE_MASK,            ITEM_MASK_GIBDO,                 GI_MASK_GIBDO,               GID_MASK_GIBDO),
    RI(RI_MASK_GORON,                 "the",  "Goron Mask",                 RITYPE_MASK,            ITEM_MASK_GORON,                 GI_MASK_GORON,               GID_MASK_GORON),
    RI(RI_MASK_GREAT_FAIRY,           "the",  "Great Fairy Mask",           RITYPE_MASK,            ITEM_MASK_GREAT_FAIRY,           GI_MASK_GREAT_FAIRY,         GID_MASK_GREAT_FAIRY),
    RI(RI_MASK_KAFEIS_MASK,           "",     "Kafei's Mask",               RITYPE_MASK,            ITEM_MASK_KAFEIS_MASK,           GI_MASK_KAFEIS_MASK,         GID_MASK_KAFEIS_MASK),
    RI(RI_MASK_KAMARO,                "",     "Kamaro's Mask",              RITYPE_MASK,            ITEM_MASK_KAMARO,                GI_MASK_KAMARO,              GID_MASK_KAMARO),
    RI(RI_MASK_KEATON,                "the",  "Keaton Mask",                RITYPE_MASK,            ITEM_MASK_KEATON,                GI_MASK_KEATON,              GID_MASK_KEATON),
    RI(RI_MASK_POSTMAN,               "the",  "Postman's Hat",              RITYPE_MASK,            ITEM_MASK_POSTMAN,               GI_MASK_POSTMAN,             GID_MASK_POSTMAN),
    RI(RI_MASK_ROMANI,                "",     "Romani's Mask",              RITYPE_MASK,            ITEM_MASK_ROMANI,                GI_MASK_ROMANI,              GID_MASK_ROMANI),
    RI(RI_MASK_SCENTS,                "the",  "Mask of Scents",             RITYPE_MASK,            ITEM_MASK_SCENTS,                GI_MASK_SCENTS,              GID_MASK_SCENTS),
    RI(RI_MASK_STONE,                 "the",  "Stone Mask",                 RITYPE_MASK,            ITEM_MASK_STONE,                 GI_MASK_STONE,               GID_MASK_STONE),
    RI(RI_MASK_TRUTH,                 "the",  "Mask of Truth",              RITYPE_MASK,            ITEM_MASK_TRUTH,                 GI_MASK_TRUTH,               GID_MASK_TRUTH),
    RI(RI_MASK_ZORA,                  "the",  "Zora Mask",                  RITYPE_MASK,            ITEM_MASK_ZORA,                  GI_MASK_ZORA,                GID_MASK_ZORA),
    RI(RI_MILK_REFILL,                "a",    "Milk Refill",                RITYPE_JUNK,            ITEM_MILK,                       GI_MILK,                     GID_MILK),
    RI(RI_MOONS_TEAR,                 "the",  "Moon's Tear",                RITYPE_MAJOR,           ITEM_MOONS_TEAR,                 GI_MOONS_TEAR,               GID_MOONS_TEAR),
    RI(RI_MUSHROOM,                   "a",    "Magic Mushroom",             RITYPE_MAJOR,           ITEM_MUSHROOM,                   GI_MUSHROOM,                 GID_MUSHROOM),
    RI(RI_NONE,                       "",     "literally nothing",          RITYPE_JUNK,            ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_OCARINA,                    "the",  "Ocarina of Time",            RITYPE_MAJOR,           ITEM_OCARINA_OF_TIME,            GI_OCARINA_OF_TIME,          GID_OCARINA),
    RI(RI_OWL_CLOCK_TOWN_SOUTH,       "the",  "Clock Town Owl Statue",      RITYPE_LESSER,          ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_OWL_GREAT_BAY_COAST,        "the",  "Great Bay Coast Owl Statue", RITYPE_LESSER,          ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_OWL_IKANA_CANYON,           "the",  "Ikana Canyon Owl Statue",    RITYPE_LESSER,          ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_OWL_MILK_ROAD,              "the",  "Milk Road Owl Statue",       RITYPE_LESSER,          ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_OWL_MOUNTAIN_VILLAGE,       "the",  "Mountain Village Owl Statue",RITYPE_LESSER,          ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_OWL_SNOWHEAD,               "the",  "Snowhead Owl Statue",        RITYPE_LESSER,          ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_OWL_SOUTHERN_SWAMP,         "the",  "Southern Swamp Owl Statue",  RITYPE_LESSER,          ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_OWL_STONE_TOWER,            "the",  "Stone Tower Owl Statue",     RITYPE_LESSER,          ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_OWL_WOODFALL,               "the",  "Woodfall Owl Statue",        RITYPE_LESSER,          ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_OWL_ZORA_CAPE,              "the",  "Zora Cape Owl Statue",       RITYPE_LESSER,          ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_PENDANT_OF_MEMORIES,        "the",  "Pendant of Memories",        RITYPE_MAJOR,           ITEM_PENDANT_OF_MEMORIES,        GI_PENDANT_OF_MEMORIES,      GID_PENDANT_OF_MEMORIES),
    RI(RI_PICTOGRAPH_BOX,             "a",    "Pictograph Box",             RITYPE_MAJOR,           ITEM_PICTOGRAPH_BOX,             GI_PICTOGRAPH_BOX,           GID_PICTOGRAPH_BOX),
    RI(RI_POWDER_KEG,                 "a",    "Powder Keg",                 RITYPE_MAJOR,           ITEM_POWDER_KEG,                 GI_POWDER_KEG,               GID_POWDER_KEG),
    RI(RI_PROGRESSIVE_BOMB_BAG,       "a",    "Progressive Bomb Bag",       RITYPE_MAJOR,           ITEM_BOMB_BAG_20,                GI_BOMB_BAG_20,              GID_BOMB_BAG_20),
    RI(RI_PROGRESSIVE_BOW,            "a",    "Progressive Bow",            RITYPE_MAJOR,           ITEM_BOW,                        GI_QUIVER_30,                GID_BOW),
    RI(RI_PROGRESSIVE_LULLABY,        "",     "Progressive Goron Lullaby",  RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_PROGRESSIVE_MAGIC,          "",     "Progressive Magic",          RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_MAGIC_JAR_SMALL),
    RI(RI_PROGRESSIVE_SWORD,          "a",    "Progressive Sword",          RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_SWORD_KOKIRI),
    RI(RI_PROGRESSIVE_WALLET,         "a",    "Progressive Wallet",         RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_WALLET_ADULT),
    RI(RI_QUIVER_40,                  "the",  "Large Quiver",               RITYPE_LESSER,          ITEM_QUIVER_40,                  GI_QUIVER_40,                GID_QUIVER_40),
    RI(RI_QUIVER_50,                  "the",  "Largest Quiver",             RITYPE_LESSER,          ITEM_QUIVER_50,                  GI_QUIVER_50,                GID_QUIVER_50),
    RI(RI_RECOVERY_HEART,             "a",    "Recovery Heart",             RITYPE_JUNK,            ITEM_RECOVERY_HEART,             GI_RECOVERY_HEART,           GID_RECOVERY_HEART),
    RI(RI_RED_POTION_REFILL,          "a",    "Red Potion Refill",          RITYPE_JUNK,            ITEM_POTION_RED,                 GI_POTION_RED,               GID_POTION_RED),
    RI(RI_REMAINS_GOHT,               "",     "Goht's Remains",             RITYPE_MAJOR,           ITEM_REMAINS_GOHT,               GI_REMAINS_GOHT,             GID_REMAINS_GOHT),
    RI(RI_REMAINS_GYORG,              "",     "Gyorg's Remains",            RITYPE_MAJOR,           ITEM_REMAINS_GYORG,              GI_REMAINS_GYORG,            GID_REMAINS_GYORG),
    RI(RI_REMAINS_ODOLWA,             "",     "Odolwa's Remains",           RITYPE_MAJOR,           ITEM_REMAINS_ODOLWA,             GI_REMAINS_ODOLWA,           GID_REMAINS_ODOLWA),
    RI(RI_REMAINS_TWINMOLD,           "",     "Twinmold's Remains",         RITYPE_MAJOR,           ITEM_REMAINS_TWINMOLD,           GI_REMAINS_TWINMOLD,         GID_REMAINS_TWINMOLD),
    RI(RI_ROOM_KEY,                   "the",  "Room Key",                   RITYPE_MAJOR,           ITEM_ROOM_KEY,                   GI_ROOM_KEY,                 GID_ROOM_KEY),
    RI(RI_RUPEE_BLUE,                 "a",    "Blue Rupee",                 RITYPE_JUNK,            ITEM_RUPEE_BLUE,                 GI_RUPEE_BLUE,               GID_RUPEE_BLUE),
    RI(RI_RUPEE_GREEN,                "a",    "Green Rupee",                RITYPE_JUNK,            ITEM_RUPEE_GREEN,                GI_RUPEE_GREEN,              GID_RUPEE_GREEN),
    RI(RI_RUPEE_HUGE,                 "a",    "Huge Rupee",                 RITYPE_JUNK,            ITEM_RUPEE_HUGE,                 GI_RUPEE_HUGE,               GID_RUPEE_HUGE),
    RI(RI_RUPEE_PURPLE,               "a",    "Purple Rupee",               RITYPE_JUNK,            ITEM_RUPEE_PURPLE,               GI_RUPEE_PURPLE,             GID_RUPEE_PURPLE),
    RI(RI_RUPEE_RED,                  "a",    "Red Rupee",                  RITYPE_JUNK,            ITEM_RUPEE_RED,                  GI_RUPEE_RED,                GID_RUPEE_RED),
    RI(RI_RUPEE_SILVER,               "a",    "Silver Rupee",               RITYPE_JUNK,            ITEM_RUPEE_SILVER,               GI_RUPEE_SILVER,             GID_RUPEE_SILVER),
    RI(RI_SHIELD_HERO,                "the",  "Hero's Shield",              RITYPE_MAJOR,           ITEM_SHIELD_HERO,                GI_SHIELD_HERO,              GID_SHIELD_HERO),
    RI(RI_SHIELD_MIRROR,              "the",  "Mirror Shield",              RITYPE_MAJOR,           ITEM_SHIELD_MIRROR,              GI_SHIELD_MIRROR,            GID_SHIELD_MIRROR),
    RI(RI_SINGLE_MAGIC,               "the",  "Power of Magic",             RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_MAGIC_JAR_SMALL),
    RI(RI_SNOWHEAD_BOSS_KEY,          "the",  "Snowhead Boss Key",          RITYPE_BOSS_KEY,        ITEM_KEY_BOSS,                   GI_KEY_BOSS,                 GID_KEY_BOSS),
    RI(RI_SNOWHEAD_COMPASS,           "the",  "Snowhead Compass",           RITYPE_LESSER,          ITEM_COMPASS,                    GI_COMPASS,                  GID_COMPASS),
    RI(RI_SNOWHEAD_MAP,               "the",  "Snowhead Map",               RITYPE_LESSER,          ITEM_DUNGEON_MAP,                GI_MAP,                      GID_DUNGEON_MAP),
    RI(RI_SNOWHEAD_SMALL_KEY,         "a",    "Snowhead Small Key",         RITYPE_SMALL_KEY,       ITEM_KEY_SMALL,                  GI_KEY_SMALL,                GID_KEY_SMALL),
    RI(RI_SNOWHEAD_STRAY_FAIRY,       "a",    "Snowhead Stray Fairy",       RITYPE_STRAY_FAIRY,     ITEM_STRAY_FAIRIES,              GI_STRAY_FAIRY,              GID_NONE),
    RI(RI_SONG_ELEGY,                 "the",  "Elegy of Emptiness",         RITYPE_MAJOR,           ITEM_SONG_ELEGY,                 GI_NONE,                     GID_NONE),
    RI(RI_SONG_EPONA,                 "",     "Epona's Song",               RITYPE_MAJOR,           ITEM_SONG_EPONA,                 GI_NONE,                     GID_NONE),
    RI(RI_SONG_HEALING,               "the",  "Song of Healing",            RITYPE_MAJOR,           ITEM_SONG_HEALING,               GI_NONE,                     GID_NONE),
    RI(RI_SONG_LULLABY_INTRO,         "the",  "Goron Lullaby Intro",        RITYPE_MAJOR,           ITEM_SONG_LULLABY_INTRO,         GI_NONE,                     GID_NONE),
    RI(RI_SONG_LULLABY,               "the",  "Goron Lullaby",              RITYPE_MAJOR,           ITEM_SONG_LULLABY,               GI_NONE,                     GID_NONE),
    RI(RI_SONG_NOVA,                  "the",  "New Wave Bossa Nova",        RITYPE_MAJOR,           ITEM_SONG_NOVA,                  GI_NONE,                     GID_NONE),
    RI(RI_SONG_OATH,                  "the",  "Oath to Order",              RITYPE_MAJOR,           ITEM_SONG_OATH,                  GI_NONE,                     GID_NONE),
    RI(RI_SONG_SOARING,               "the",  "Song of Soaring",            RITYPE_MAJOR,           ITEM_SONG_SOARING,               GI_NONE,                     GID_NONE),
    RI(RI_SONG_SONATA,                "the",  "Sonata of Awakening",        RITYPE_MAJOR,           ITEM_SONG_SONATA,                GI_NONE,                     GID_NONE),
    RI(RI_SONG_STORMS,                "the",  "Song of Storms",             RITYPE_MAJOR,           ITEM_SONG_STORMS,                GI_NONE,                     GID_NONE),
    RI(RI_SONG_SUN,                   "the",  "Sun's Song",                 RITYPE_MAJOR,           ITEM_SONG_SUN,                   GI_NONE,                     GID_NONE),
    RI(RI_SONG_TIME,                  "the",  "Song of Time",               RITYPE_MAJOR,           ITEM_SONG_TIME,                  GI_NONE,                     GID_NONE),
    RI(RI_SOUL_ARMOS,                 "the",  "Soul of Armos",              RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_SOUL_BAT,                   "the",  "Soul of Bats",               RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_SOUL_BOMBCHU,               "the",  "Soul of Bombchus",           RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_SOUL_DEATH_ARMOS,           "the",  "Soul of Death Armos",        RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_SOUL_DEKU_BABA,             "the",  "Soul of Deku Babas",         RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_SOUL_DINOLFOS,              "the",  "Soul of Dinolfos",           RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_SOUL_DODONGO,               "the",  "Soul of Dodongos",           RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_SOUL_GARO,                  "the",  "Soul of Garos",              RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_SOUL_KEESE,                 "the",  "Soul of Keese",              RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_SOUL_LEEVER,                "the",  "Soul of Leevers",            RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_SOUL_MAD_SCRUB,             "the",  "Soul of Mad Scrubs",         RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_SOUL_OCTOROK,               "the",  "Soul of Octoroks",           RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_SOUL_PEEHAT,                "the",  "Soul of Peehats",            RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_SOUL_SKULLTULA,             "the",  "Soul of Skulltulas",         RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_SOUL_SLIME,                 "the",  "Soul of Slimes",             RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_SOUL_TEKTITE,               "the",  "Soul of Tektites",           RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_SOUL_WALLMASTER,            "the",  "Soul of Wallmasters",        RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_SOUL_WOLFOS,                "the",  "Soul of Wolfos",             RITYPE_MAJOR,           ITEM_NONE,                       GI_NONE,                     GID_NONE),
    RI(RI_STONE_TOWER_BOSS_KEY,       "the",  "Stone Tower Boss Key",       RITYPE_BOSS_KEY,        ITEM_KEY_BOSS,                   GI_KEY_BOSS,                 GID_KEY_BOSS),
    RI(RI_STONE_TOWER_COMPASS,        "the",  "Stone Tower Compass",        RITYPE_LESSER,          ITEM_COMPASS,                    GI_COMPASS,                  GID_COMPASS),
    RI(RI_STONE_TOWER_MAP,            "the",  "Stone Tower Map",            RITYPE_LESSER,          ITEM_DUNGEON_MAP,                GI_MAP,                      GID_DUNGEON_MAP),
    RI(RI_STONE_TOWER_SMALL_KEY,      "a",    "Stone Tower Small Key",      RITYPE_SMALL_KEY,       ITEM_KEY_SMALL,                  GI_KEY_SMALL,                GID_KEY_SMALL),
    RI(RI_STONE_TOWER_STRAY_FAIRY,    "a",    "Stone Tower Stray Fairy",    RITYPE_STRAY_FAIRY,     ITEM_STRAY_FAIRIES,              GI_STRAY_FAIRY,              GID_NONE),
    RI(RI_SWORD_GILDED,               "the",  "Gilded Sword",               RITYPE_LESSER,          ITEM_SWORD_GILDED,               GI_SWORD_GILDED,             GID_SWORD_GILDED),
    RI(RI_SWORD_KOKIRI,               "the",  "Kokiri Sword",               RITYPE_MAJOR,           ITEM_SWORD_KOKIRI,               GI_SWORD_KOKIRI,             GID_SWORD_KOKIRI),
    RI(RI_SWORD_RAZOR,                "the",  "Razor Sword",                RITYPE_LESSER,          ITEM_SWORD_RAZOR,                GI_SWORD_RAZOR,              GID_SWORD_RAZOR),
    RI(RI_TINGLE_MAP_CLOCK_TOWN,      "",     "Tingle's Clock Town Map",    RITYPE_LESSER,          ITEM_TINGLE_MAP,                 GI_TINGLE_MAP_CLOCK_TOWN,    GID_TINGLE_MAP),
    RI(RI_TINGLE_MAP_GREAT_BAY,       "",     "Tingle's Great Bay Map",     RITYPE_LESSER,          ITEM_TINGLE_MAP,                 GI_TINGLE_MAP_GREAT_BAY,     GID_TINGLE_MAP),
    RI(RI_TINGLE_MAP_ROMANI_RANCH,    "",     "Tingle's Romani Ranch Map",  RITYPE_LESSER,          ITEM_TINGLE_MAP,                 GI_TINGLE_MAP_ROMANI_RANCH,  GID_TINGLE_MAP),
    RI(RI_TINGLE_MAP_SNOWHEAD,        "",     "Tingle's Snowhead Map",      RITYPE_LESSER,          ITEM_TINGLE_MAP,                 GI_TINGLE_MAP_SNOWHEAD,      GID_TINGLE_MAP),
    RI(RI_TINGLE_MAP_STONE_TOWER,     "",     "Tingle's Stone Tower Map",   RITYPE_LESSER,          ITEM_TINGLE_MAP,                 GI_TINGLE_MAP_STONE_TOWER,   GID_TINGLE_MAP),
    RI(RI_TINGLE_MAP_WOODFALL,        "",     "Tingle's Woodfall Map",      RITYPE_LESSER,          ITEM_TINGLE_MAP,                 GI_TINGLE_MAP_WOODFALL,      GID_TINGLE_MAP),
    RI(RI_WALLET_ADULT,               "the",  "Adult's Wallet",             RITYPE_MAJOR,           ITEM_WALLET_ADULT,               GI_WALLET_ADULT,             GID_WALLET_ADULT),
    RI(RI_WALLET_GIANT,               "the",  "Giant's Wallet",             RITYPE_LESSER,          ITEM_WALLET_GIANT,               GI_WALLET_GIANT,             GID_WALLET_GIANT),
    RI(RI_WOODFALL_BOSS_KEY,          "the",  "Woodfall Boss Key",          RITYPE_BOSS_KEY,        ITEM_KEY_BOSS,                   GI_KEY_BOSS,                 GID_KEY_BOSS),
    RI(RI_WOODFALL_COMPASS,           "the",  "Woodfall Compass",           RITYPE_LESSER,          ITEM_COMPASS,                    GI_COMPASS,                  GID_COMPASS),
    RI(RI_WOODFALL_MAP,               "the",  "Woodfall Map",               RITYPE_LESSER,          ITEM_DUNGEON_MAP,                GI_MAP,                      GID_DUNGEON_MAP),
    RI(RI_WOODFALL_SMALL_KEY,         "a",    "Woodfall Small Key",         RITYPE_SMALL_KEY,       ITEM_KEY_SMALL,                  GI_KEY_SMALL,                GID_KEY_SMALL),
    RI(RI_WOODFALL_STRAY_FAIRY,       "a",    "Woodfall Stray Fairy",       RITYPE_STRAY_FAIRY,     ITEM_STRAY_FAIRIES,              GI_STRAY_FAIRY,              GID_NONE),
};

RandoItemId GetItemIdFromName(const char* name) {
    for (auto& [randoItemId, randoStaticItem] : Items) {
        if (strcmp(name, randoStaticItem.spoilerName) == 0) {
            return randoItemId;
        }
    }
    return RI_UNKNOWN;
}

// This exists because of nintendo being nintendo
u8 GetIconForZMessage(RandoItemId randoItemId) {
    switch (randoItemId) {
        case RI_MASK_CAPTAIN:
            return GI_MASK_TRUTH;
        case RI_MASK_TRUTH:
            return GI_MASK_CAPTAIN;
        case RI_MASK_GIANT:
            return GI_MASK_KAFEIS_MASK;
        case RI_MASK_KAFEIS_MASK:
            return GI_MASK_GIANT;
        case RI_BOMBCHU:
        case RI_BOMBCHU_5:
            return GI_BOMBCHUS_10;
        case RI_BOW:
            return GI_ARROWS_10;
        case RI_DEKU_STICKS_5:
            return GI_DEKU_STICKS_1;
        case RI_DOUBLE_DEFENSE:
            return GI_HEART_CONTAINER;
        case RI_SINGLE_MAGIC:
            return GI_MAGIC_JAR_SMALL;
        case RI_DOUBLE_MAGIC:
            return GI_MAGIC_JAR_BIG;
        case RI_GREAT_SPIN_ATTACK:
            return GI_SWORD_KOKIRI;
        default:
            break;
    }

    if (Rando::StaticData::Items[randoItemId].getItemId != GI_NONE) {
        return (u8)Rando::StaticData::Items[randoItemId].getItemId;
    }

    return 0xFE;
}

const char* GetIconTexturePath(RandoItemId randoItemId) {
    switch (randoItemId) {
        case RI_BOMBCHU:
        case RI_BOMBCHU_5:
            return (const char*)gItemIcons[ITEM_BOMBCHU];
        case RI_DEKU_STICKS_5:
            return (const char*)gItemIcons[ITEM_DEKU_STICK];
        case RI_DOUBLE_DEFENSE:
            return (const char*)gItemIcons[ITEM_HEART_CONTAINER];
        case RI_SINGLE_MAGIC:
            return (const char*)gItemIcons[ITEM_MAGIC_JAR_SMALL];
        case RI_DOUBLE_MAGIC:
            return (const char*)gItemIcons[ITEM_MAGIC_JAR_BIG];
        case RI_GREAT_SPIN_ATTACK:
            return (const char*)gItemIcons[ITEM_SWORD_KOKIRI];
        case RI_CLOCK_TOWN_STRAY_FAIRY:
        case RI_STONE_TOWER_STRAY_FAIRY:
            return (const char*)gStrayFairyStoneTowerIconTex;
        case RI_SNOWHEAD_STRAY_FAIRY:
            return (const char*)gStrayFairySnowheadIconTex;
        case RI_GREAT_BAY_STRAY_FAIRY:
            return (const char*)gStrayFairyGreatBayIconTex;
        case RI_WOODFALL_STRAY_FAIRY:
            return (const char*)gStrayFairyWoodfallIconTex;
        case RI_SNOWHEAD_COMPASS:
        case RI_GREAT_BAY_COMPASS:
        case RI_WOODFALL_COMPASS:
        case RI_STONE_TOWER_COMPASS:
            return (const char*)gItemIcons[ITEM_DUNGEON_MAP];
        case RI_SNOWHEAD_MAP:
        case RI_GREAT_BAY_MAP:
        case RI_WOODFALL_MAP:
        case RI_STONE_TOWER_MAP:
            return (const char*)gItemIcons[ITEM_COMPASS];
        default:
            break;
    }

    s16 itemId = Rando::StaticData::Items[randoItemId].itemId;
    if (itemId >= ITEM_RECOVERY_HEART) {
        itemId = D_801CFF94[Rando::StaticData::Items[randoItemId].getItemId];
    }

    return itemId < ITEM_RECOVERY_HEART ? (const char*)gItemIcons[itemId] : nullptr;
}

bool ShouldShowGetItemCutscene(RandoItemId itemId) {
    if (!CVarGetInteger("gEnhancements.Cutscenes.SkipGetItemCutscenes", 0)) {
        return true;
    }

    switch (Rando::StaticData::Items[itemId].randoItemType) {
        case RITYPE_JUNK:
            return CVarGetInteger("gEnhancements.Cutscenes.SkipGetItemCutscenes", 0) < 1;
        case RITYPE_HEALTH:
        case RITYPE_LESSER:
        case RITYPE_STRAY_FAIRY:
        case RITYPE_SKULLTULA_TOKEN:
        case RITYPE_SMALL_KEY:
            return CVarGetInteger("gEnhancements.Cutscenes.SkipGetItemCutscenes", 0) < 2;
        default:
            return CVarGetInteger("gEnhancements.Cutscenes.SkipGetItemCutscenes", 0) < 3;
    }
}

// clang-format on

} // namespace StaticData

} // namespace Rando
