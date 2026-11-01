#pragma once


#include <cstdarg>
#include <cstring>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


// ObjType.txt and ObjMode.txt are not shipped as compiled .bin files, so the records are built from the 1.10f ObjType.txt (without the Expansion row) and ObjMode.txt
template<class Fixture>
struct ObjModeTypeTxtFixture : Fixture
{
	std::unique_ptr<D2ObjModeTypeTxt[]> objmodetype_txt;
	int objtype_record_count;
	int objmode_record_count;
	uintptr_t objmodetype_d2common_base;

	ObjModeTypeTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));
		objmodetype_d2common_base = d2common_base;

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		constexpr const char* objtype[][2] = {
			{ "Not Used", "NU0" }, { "Casket #5", "C5" }, { "Outdoor Shrine", "SF" }, { "Casket #6", "C6" },
			{ "Urn #1", "U1" }, { "Large Chest R", "L1" }, { "Large Chest L", "L2" }, { "Barrel", "B1" },
			{ "Tower Tome", "TT" }, { "Urn #2", "U2" }, { "Bench", "BE" }, { "Barrel Exploding", "BX" },
			{ "Fountain", "FN" }, { "Door Gate Left", "D1" }, { "Door Gate Right", "D2" }, { "Door Wooden Left", "D3" },
			{ "Door Wooden Right", "D4" }, { "Stone 1", "S1" }, { "Stone 2", "S2" }, { "Stone 3", "S3" },
			{ "Stone 4", "S4" }, { "Stone 5", "S5" }, { "Stone 6", "S6" }, { "Door Courtyard Left", "D5" },
			{ "Door Courtyard Right", "D6" }, { "Door Cathedral", "D7" }, { "Gibbet", "GI" }, { "Door Monastery", "D8" },
			{ "Hole in Ground", "HI" }, { "Brazier", "BR" }, { "Inifuss Tree", "IT" }, { "Blood Fountain", "BF" },
			{ "Crucifix Large", "CL" }, { "Candles 1", "A1" }, { "Candles 2", "A2" }, { "Standard 1", "N1" },
			{ "Standard 2", "N2" }, { "Tiki Torch", "TO" }, { "Wall Torch", "WT" }, { "Rogue Bonfire", "RB" },
			{ "River 1", "R1" }, { "River 2", "R2" }, { "River 3", "R3" }, { "River 4", "R4" }, { "River 5", "R5" },
			{ "Town 1 Ambient", "SS" }, { "Crate", "CT" }, { "Andariel Door", "AD" }, { "Rogue Torch 1", "T1" },
			{ "Rogue Torch 2", "T2" }, { "Casket 1", "C1" }, { "Casket 2", "C2" }, { "Urn #3", "U3" },
			{ "Casket Raised", "C4" }, { "Rogue Corpse", "Z1" }, { "Rogue Corpse", "Z2" },
			{ "Rogue Rolling Corpse", "Z5" }, { "Rogue Staked 1", "Z3" }, { "Rogue Staked 2", "Z4" },
			{ "Town Portal", "TP" }, { "Portal Permanent", "PP" }, { "Stone Sound", "SS" },
			{ "Door Small Cathedral", "D9" }, { "Door Small Cathedral", "DA" }, { "Wooden Door", "DB" },
			{ "River Sound Only 1", "X1" }, { "River Sound Only 1", "X2" }, { "Ripple 1", "1R" }, { "Ripple 2", "2R" },
			{ "Ripple 3", "3R" }, { "Ripple 4", "4R" }, { "Forest Night 1", "F1" }, { "Forest Night 2", "F2" },
			{ "Yeti Dung", "YD" }, { "Trap Door", "TD" }, { "Dock Door ", "DD" }, { "Sewer Drip", "SZ" },
			{ "ShrineHealth", "SH" }, { "Town Ambient", "SS" }, { "Casket 3", "C3" }, { "Obelisk 1", "OB" },
			{ "Forest Altar", "AF" }, { "Bubbling pool of blood", "B2" }, { "Horn Shrine", "HS" },
			{ "Healing Well", "HW" }, { "Bull Shrine", "BC" }, { "stele", "SG" }, { "tomb chest 1", "CA" },
			{ "tomb chest 2", "CB" }, { "mummy coffin", "MC" }, { "desert obelisk", "DO" }, { "tomb door left", "TL" },
			{ "tomb door right", "TR" }, { "innerhellmana", "iz" }, { "urn #4", "U4" }, { "urn #5", "U5" },
			{ "innerhellhealth", "iy" }, { "innershrine1", "ix" }, { "tomb door left 2", "TS" },
			{ "tomb door right 2", "TU" }, { "secret door #1", "SJ" }, { "Brazier", "B3" }, { "Floor brazier", "FB" },
			{ "flies", "FL" }, { "Armor Stand 1", "A3" }, { "Armor Stand 2", "A4" }, { "Weapon Rack 1", "W1" },
			{ "Weapon Rack 2", "W2" }, { "Malus", "HM" }, { "palace shrine", "P2" }, { "not used", "n5" },
			{ "Fountain 1", "F3" }, { "not used", "n6" }, { "Fountain 2", "F4" }, { "not used", "n7" },
			{ "Fountain 3", "F5" }, { "Snake woman", "SN" }, { "jungle torch", "JT" }, { "Fountain 4", "F6" },
			{ "waypoint portal", "wp" }, { "dungeonhealwell", "dj" }, { "jerhyn", "ss" }, { "jerhyn ", "ss" },
			{ "innerhellshrine2", "iw" }, { "innerhellshrine3", "iv" }, { "innerhellobject3", "iu" },
			{ "innerhellobject4", "is" }, { "innerhellobject5", "ir" }, { "hellobject4", "hg" }, { "hidden door 2", "h2" },
			{ "well", "zw" }, { "zzz", "9b" }, { "cwell", "zc" }, { "arcane shrine", "az" }, { "dshrine2", "zs" },
			{ "shrine3", "zr" }, { "dshrine1", "zd" }, { "dwell", "zl" }, { "cavwell", "zy" }, { "chest2", "q1" },
			{ "chest3", "q2" }, { "chest1", "q3" }, { "jug1", "q4" }, { "jug2", "q5" }, { "Lchest1", "q6" },
			{ "waypointinnerhell", "wi" }, { "dchest2", "q9" }, { "dchestr", "q7" }, { "dchestl", "q8" },
			{ "sptomb", "za" }, { "templeshrine", "zv" }, { "dshrine4", "ze" }, { "seven tomb receptacle", "HA" },
			{ "duriels door", "DX" }, { "guard corpse", "GC" }, { "rock", "c7" }, { "waypoint", "wm" },
			{ "waypoint", "wn" }, { "corpse", "cp" }, { "rockb", "cq" }, { "fire small", "FX" }, { "fire medium", "FY" },
			{ "fire large", "FZ" }, { "cliff", "cf" }, { "mana well1", "MB" }, { "mana well2", "MD" },
			{ "mana well3", "MF" }, { "mana well4", "MH" }, { "mana well5", "MJ" }, { "log", "cz" },
			{ "jungle healwell", "JH" }, { "corpseb", "sx" }, { "mana well6", "Mk" }, { "mana well7", "Mi" },
			{ "rockc", "RY" }, { "rockd", "RZ" }, { "chesst", "c8" }, { "chest", "c9" }, { "guardonastick", "GS" },
			{ "bookshelf1", "b4" }, { "bookshelf2", "b5" }, { "jungle chest", "JC" }, { "coffin", "tm" },
			{ "jchest", "jz" }, { "jshrine", "jy" }, { "jobject1", "jx" }, { "jobject3", "jw" }, { "jobject4", "jv" },
			{ "jobject5", "ju" }, { "cain portal", "tp" }, { "jshrine3", "js" }, { "jshrine4", "jr" },
			{ "teleport pad", "7h" }, { "lam esen's tome", "ab" }, { "dstairL", "9c" }, { "dstairr", "sv" },
			{ "test object trap melee", "a5" }, { "jshrinewood", "jq" }, { "chest", "c0" }, { "mafistoshrine1", "mz" },
			{ "mafistoshrine2", "my" }, { "mafistoshrine3", "mx" }, { "mafistomana", "mw" }, { "mobject1", "mv" },
			{ "mobject2", "mu" }, { "mobject3", "mt" }, { "mafistohealth", "mr" }, { "wrok", "rw" }, { "basket 1", "bd" },
			{ "basket 2", "bj" }, { "logw", "lw" }, { "wrob", "wb" }, { "bubbles", "yb" }, { "logx", "wd" },
			{ "rokb", "wc" }, { "watc", "we" }, { "waty", "wy" }, { "logz", "lx" }, { "webtree1", "w3" },
			{ "webtree2", "w4" }, { "webtree3", "w5" }, { "webtree4", "w6" }, { "hobject1", "70" }, { "cacoon", "CN" },
			{ "cacoon 2", "CC" }, { "hobject3", "ib" }, { "hshrine1", "ia" }, { "blgb", "QX" }, { "blga", "qw" },
			{ "slimedoor1", "SQ" }, { "slimedoor2", "SY" }, { "hshrine2", "ht" }, { "hshrine3", "hq" },
			{ "hobject2", "hv" }, { "blgc", "Qy" }, { "blgd", "Qz" }, { "hhealthwell", "ho" }, { "waypointj", "wz" },
			{ "waypointh", "wv" }, { "burningbody", "bz" }, { "gchest1", "cy" }, { "gchest2", "cx" }, { "gchest3", "cu" },
			{ "glchest3", "cd" }, { "rnest", "rn" }, { "burningbody2", "by" }, { "ratchest", "ra" }, { "beda", "qa" },
			{ "bedb", "qb" }, { "hell mana shrine", "hn" }, { "cowa", "ew" }, { "gidbinn altar", "ga" },
			{ "gidbinn decoy", "gd" }, { "right light", "11" }, { "left light", "12" }, { "diablo start", "ss" },
			{ "stol", "s9" }, { "wdda", "wg" }, { "wddb", "wh" }, { "skla", "QS" }, { "holyshrine", "HL" },
			{ "tombspikes", "A7" }, { "cshrine1", "s0" }, { "jashrine1", "jb" }, { "jahshrine", "jd" },
			{ "jamshrine", "jf" }, { "goopile", "GP" }, { "bank", "b6" }, { "wirt's corpse", "BP" },
			{ "gold placement ", "1g" }, { "guardcorpse2", "GF" }, { "dead villager 1", "dg" },
			{ "dead villager 2", "df" }, { "flame1", "f8" }, { "wumpus", "f9" }, { "caveheal shrine", "ce" },
			{ "cavemana shrine", "cg" }, { "caveshrine", "cj" }, { "dungeonmanawell", "de" }, { "magicshrine", "wj" },
			{ "sewerhealwell", "wk" }, { "sewermanawell", "wl" }, { "sewershrine2", "ws" }, { "brazier", "bi" },
			{ "anubiscoffin", "QC" }, { "brazier", "bm" }, { "brazier", "bo" }, { "brazier", "bq" }, { "waypoint", "w7" },
			{ "ubed", "ub" }, { "iron grate door left", "dv" }, { "iron grate door right", "dn" },
			{ "wooden grate door left", "dp" }, { "wooden grate door right", "dt" }, { "wooden door left", "dk" },
			{ "wooden door right", "dl" }, { "tombwalltorchL", "qd" }, { "tombwalltorchR", "qe" },
			{ "arcane sanctuary portal", "ay" }, { "haramshrine1", "hb" }, { "haramshrine2", "hc" },
			{ "maggotwellheal", "qf" }, { "maggotwellmana", "qg" }, { "arcanesantuaryshrine", "hd" },
			{ "teleportpad", "7h" }, { "teleportpad", "aa" }, { "teleportpad", "aa" }, { "arcane rune #1", "7a" },
			{ "arcane rune #2", "7b" }, { "arcane rune #3", "7c" }, { "arcane rune #4", "7d" }, { "arcane rune #5", "7e" },
			{ "arcane rune #6", "7f" }, { "arcane rune #7", "7g" }, { "harem guard 1", "qh" }, { "harem guard 2", "qi" },
			{ "harem guard 3", "qj" }, { "harem guard 4", "qk" }, { "eunuch", "ss" }, { "ahshrine", "ax" },
			{ "amshrine", "au" }, { "Not Used", "pp" }, { "twell", "hu" }, { "swaypoint", "qm" }, { "twaypoint", "ql" },
			{ "s3shrine", "qn" }, { "ddbody", "qo" }, { "stra", "V1" }, { "strb", "V2" }, { "mchest1L", "xb" },
			{ "mchest1R", "xc" }, { "mchest2L", "xd" }, { "mchest2R", "xe" }, { "schest1L", "xf" }, { "schest3L", "xg" },
			{ "schest2r", "xh" }, { "schest3r", "xi" }, { "steeg stone", "y6" }, { "guild vault", "y4" },
			{ "trophy case", "y2" }, { "message board", "y3" }, { "mbridge", "xj" }, { "hellgate", "1y" },
			{ "kmshrine", "xl" }, { "khshrine", "xm" }, { "hellfire1", "e3" }, { "hellfire2", "e4" },
			{ "hellfire3", "e5" }, { "helllava1", "e6" }, { "helllava2", "e7" }, { "helllava3", "e8" },
			{ "helllightsource1", "ss" }, { "helllightsource2", "ss" }, { "helllightsource3", "ss" }, { "chest", "xk" },
			{ "chest", "xk" }, { "chest", "xk" }, { "tome", "tt" }, { "hell brazier", "e1" }, { "hell brazier", "e2" },
			{ "rockpile", "xn" }, { "dshrine2", "xo" }, { "dbasket", "xp" }, { "hhobject4", "xq" }, { "duda", "ea" },
			{ "dungncasket", "vb" }, { "sewerstairs", "ve" }, { "sewerlever", "vf" }, { "darkwandererstart", "ss" },
			{ "trapped placeholder", "ss" }, { "jungletorch2", "vg" }, { "forgottentowerchest", "L1" },
			{ "innerhellbone", "y1" }, { "skla", "Qt" }, { "rfga", "ud" }, { "hellbridge", "xx" }, { "fora", "ux" },
			{ "Guild portal", "pp" }, { "hratli start", "ss" }, { "hratli end", "ss" }, { "guyq", "uy" }, { "guyq", "15" },
			{ "Natalya Start", "ss" }, { "damnedsoul", "18" }, { "damnedsoul", "19" }, { "cain start", "ss" },
			{ "reallythelastobject", "sv" }, { "chest", "y7" }, { "casket", "y8" }, { "chest", "y9" }, { "chest", "ya" },
			{ "chest", "yc" }, { "diablo seal", "30" }, { "diablo seal", "31" }, { "diablo seal", "32" },
			{ "diablo seal", "33" }, { "diablo seal", "34" }, { "sparklechest", "yf" }, { "pfwaypoint", "yg" },
			{ "fissure", "fh" }, { "hellbrazier", "he" }, { "hellsmoke", "35" }, { "waypoint", "yi" },
			{ "hellbrazier", "9f" }, { "compelling orb", "55" }, { "chest", "xk" }, { "chest", "xk" }, { "chest", "xk" },
			{ "fortress brazier #1", "98" }, { "fortress brazier #2", "99" }, { "Siege Control", "zq" }, { "ptox", "px" },
			{ "pyox", "py" }, { "echest", "6q" }, { "eshrine1", "6r" }, { "eshrine2", "6s" }, { "eobject1", "3w" },
			{ "eflag", "ym" }, { "ebarrel", "yn" }, { "eexplodingbarrel", "yo" }, { "ewoodchest", "yp" },
			{ "eshrine3", "yq" }, { "emanashrine", "yr" }, { "ehealthshrine", "ys" }, { "eburialchestL", "yt" },
			{ "eburialchestR", "yu" }, { "ewell", "yv" }, { "eshrine4", "yw" }, { "eshrine2wsnow", "yx" },
			{ "ewaypoint", "yy" }, { "echestL", "yz" }, { "ewoodchestR", "6a" }, { "echestSL", "6b" },
			{ "echestSR", "6c" }, { "etorch1", "6d" }, { "ecfra", "2w" }, { "ettr", "2x" }, { "etorch1", "6e" },
			{ "eburningbodies", "6f" }, { "eburningpit", "6g" }, { "eflag", "6h" }, { "eflg", "2y" }, { "chan", "2z" },
			{ "ejar1", "6i" }, { "ejar2", "6j" }, { "ejar3", "6k" }, { "eswing", "6L" }, { "epole", "6m" },
			{ "eshit", "6n" }, { "gate", "2v" }, { "eshit2", "6o" }, { "ehgate", "6p" }, { "banner 1", "ao" },
			{ "banner 2", "ap" }, { "eexplodingchest", "6t" }, { "specialchest", "6u" }, { "deathpole", "6v" },
			{ "Ldeathpole", "6w" }, { "Altar", "6x" }, { "dummy", "ss" }, { "dummy", "ss" }, { "dummy", "ss" },
			{ "dummy", "ss" }, { "icecave_obj", "6y" }, { "icecave_health", "8a" }, { "icecave_mana", "8b" },
			{ "icecave_evilurn", "8c" }, { "icecave_jar1", "8d" }, { "icecave_jar2", "8e" }, { "icecave_jar3", "8f" },
			{ "icecave_jar4", "8g" }, { "icecave_jar5", "8h" }, { "icecave_shrine2", "8i" }, { "wussiecage", "ss" },
			{ "statue", "60" }, { "statue", "61" }, { "statue", "62" }, { "deadbarbarian", "8j" },
			{ "client smoke", "oz" }, { "icecave_shrine1", "8k" }, { "icecave_torch", "8L" }, { "icecave_torch2", "8m" },
			{ "ttor", "2p" }, { "baals_manawell", "8n" }, { "baals_healthwell", "8o" }, { "baals_tomb1", "8p" },
			{ "baals_tomb2", "8q" }, { "baals_tomb3", "8r" }, { "baals_shrine1", "8s" }, { "baals_torch1", "8t" },
			{ "baals_torch2", "8u" }, { "snowy_mana", "8v" }, { "snowy_health", "8w" }, { "snowy_well", "8x" },
			{ "baals_waypoint", "8y" }, { "snowy_shrine3", "8z" }, { "wilderness_waypoint", "5a" },
			{ "snowy_shrine4", "5b" }, { "baals_well", "5c" }, { "baals_shrine2", "5d" }, { "snowy_object1", "5e" },
			{ "snowy_woodchestL", "5f" }, { "snowy_woodchestR", "5g" }, { "baals_shrine3", "5h" },
			{ "snowy_woodchest2L", "5i" }, { "snowy_woodchest2R", "5j" }, { "snowy_hangingbodies", "5k" },
			{ "snowy_debris", "5L" }, { "pene", "2q" }, { "temple_shrine1", "5m" }, { "snowy_mrpole", "5n" },
			{ "icave_waypoint", "5o" }, { "etemple_shrine2", "5t" }, { "etemple_well", "5q" }, { "etemple_torch1", "5r" },
			{ "etemple_torch2", "5s" }, { "etemple_object1", "5u" }, { "etemple_object2", "5v" }, { "ebaals_mrbox", "5w" },
			{ "eicecave_well", "5x" }, { "etemple_shrine3", "5y" }, { "etemple_health", "5z" }, { "etemple_mana", "3a" },
			{ "dummy", "ss " }, { "ebaals_tomb1L", "3b" }, { "ebaals_tomb2L", "3c" }, { "ebaals_tomb3L", "3d" },
			{ "ubub", "2u" }, { "sbub", "2s" }, { "eredbaals_tomb1", "3f" }, { "eredbaals_tomb1L", "3g" },
			{ "eredbaals_tomb2", "3h" }, { "eredbaals_tomb2L", "3i" }, { "eredbaals_tomb3", "3j" },
			{ "eredbaals_tomb3L", "3k" }, { "eredbaals_mrbox", "3L" }, { "eredbaals_torch1", "3m" },
			{ "eredbaals_torch2", "3n" }, { "etemple_candle", "3o" }, { "etemple_waypoint", "3p" },
			{ "edeadperson", "3q" }, { "etemple_groundtomb", "3s" }, { "Larzuk Greeting", "ss " },
			{ "Larzuk Standard", "ss " }, { "etemple_groundtombL", "3t" }, { "edeadperson2", "3u" },
			{ "ancientsaltar", "4a" }, { "ancientsdoor", "4b" }, { "eweaponrackR", "3x" }, { "eweaponrackL", "3y" },
			{ "earmorstandR", "3z" }, { "earmorstandL", "4c" }, { "esummittorch", "9g" }, { "efuneralpire", "9h" },
			{ "eburninglogs", "9i" }, { "stma", "2o" }, { "edeadperson2L", "3v" }, { "client only thingie", "ss " },
			{ "fana", "2n" }, { "BBQB", "29" }, { "btor", "25" }, { "invisible ancient", "ss " }, { "collision", "ss " },
			{ "baalsportal", "4x" }, { "summitdoor", "4u" }, { "lastportal", "pp" }, { "onemorelastportal", "pp" },
			{ "zoo", "ss " }, { "keeper", "7z" }, { "baalsportal2", "4x" }, { "fireplaceguy", "7y" }, { "dummy", "ss " },
			{ "dummy", "ss " },
		};

		constexpr const char* objmode[][2] = {
			{ "Neutral", "NU" }, { "Operating", "OP" }, { "Opened", "ON" }, { "Special1", "S1" },
			{ "Special2", "S2" }, { "Special3", "S3" }, { "Special4", "S4" }, { "Special5", "S5" },
		};

		const auto type_count = static_cast<int>(std::size(objtype));
		const auto mode_count = static_cast<int>(std::size(objmode));

		auto records = std::make_unique<D2ObjModeTypeTxt[]>(type_count + mode_count);
		std::memset(records.get(), 0, sizeof(D2ObjModeTypeTxt) * (type_count + mode_count));

		const auto fill_record = [](D2ObjModeTypeTxt& record, const char* name, const char* token)
		{
			strcpy_s(record.szName, sizeof(record.szName), name);
			std::memset(record.szToken, ' ', 4);
			std::memcpy(record.szToken, token, std::strlen(token));
		};

		for (auto i = 0; i < type_count; ++i)
		{
			fill_record(records[i], objtype[i][0], objtype[i][1]);
		}

		for (auto i = 0; i < mode_count; ++i)
		{
			fill_record(records[type_count + i], objmode[i][0], objmode[i][1]);
		}

		sgptDataTables->pObjModeDataTables.nObjModeTypeTxtRecordCount = type_count + mode_count;
		sgptDataTables->pObjModeDataTables.pObjModeTypeTxt = records.get();
		sgptDataTables->pObjModeDataTables.pObjType = records.get();
		sgptDataTables->pObjModeDataTables.pObjMode = &records[type_count];

		*original_sgptDataTables = sgptDataTables;

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x000010C4);
		*original_record_count = type_count + mode_count;

		const auto original_objmodetype = reinterpret_cast<D2ObjModeTypeTxt**>(d2common_base + 0x000A9608 + 0x000010C8);
		*original_objmodetype = records.get();

		const auto original_objtype = reinterpret_cast<D2ObjModeTypeTxt**>(d2common_base + 0x000A9608 + 0x000010CC);
		*original_objtype = records.get();

		const auto original_objmode = reinterpret_cast<D2ObjModeTypeTxt**>(d2common_base + 0x000A9608 + 0x000010D0);
		*original_objmode = &records[type_count];

		objmodetype_txt = std::move(records);
		objtype_record_count = type_count;
		objmode_record_count = mode_count;
	};

	// The records are freed with the fixture, so don't leave dangling pointers behind for the following tests
	~ObjModeTypeTxtFixture()
	{
		sgptDataTables->pObjModeDataTables.nObjModeTypeTxtRecordCount = 0;
		sgptDataTables->pObjModeDataTables.pObjModeTypeTxt = nullptr;
		sgptDataTables->pObjModeDataTables.pObjType = nullptr;
		sgptDataTables->pObjModeDataTables.pObjMode = nullptr;

		*reinterpret_cast<int*>(objmodetype_d2common_base + 0x000A9608 + 0x000010C4) = 0;
		*reinterpret_cast<D2ObjModeTypeTxt**>(objmodetype_d2common_base + 0x000A9608 + 0x000010C8) = nullptr;
		*reinterpret_cast<D2ObjModeTypeTxt**>(objmodetype_d2common_base + 0x000A9608 + 0x000010CC) = nullptr;
		*reinterpret_cast<D2ObjModeTypeTxt**>(objmodetype_d2common_base + 0x000A9608 + 0x000010D0) = nullptr;
	}
};
