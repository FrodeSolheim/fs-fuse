#include "fsfuse.h"

#define MAX_ARGS 1024

static struct {
    int argc;
    char **argv;
    GHashTable *settings;
} fsfuse_config;

#if 1
void fsfuse_add_arg(const char *arg)
{
    fsfuse_config.argv[fsfuse_config.argc] = strdup(arg);
    fsfuse_config.argc += 1;
}

void fsfuse_add_arg_2(const char *arg_1, const char *arg_2)
{
    fsfuse_add_arg(arg_1);
    fsfuse_add_arg(arg_2);
}

void fsfuse_config_args(int *argc, char ***argv)
{
    *argc = fsfuse_config.argc;
    *argv = fsfuse_config.argv;
}
#endif

static void fuse_setting(const char *key, const char *value)
{
    g_hash_table_insert(fsfuse_config.settings, strdup(key), strdup(value));
}

#define OPTION_TAPE_DRIVE_0 "tape_drive_0"
#define OPTION_TAPE_DRIVE_SOUNDS "tape_drive_sounds"
#define OPTION_ZXS_MODEL "zxs_model"

extern const char *fsfuse_config_settings_keys[];

xmlDoc *fsfuse_config_create_settings_xml(void)
{
    printf("fsfuse_config_create_settings_xml\n");
    xmlDocPtr doc = xmlNewDoc((const xmlChar *) "1.0");
    xmlNodePtr settings_node = xmlNewNode(NULL, (const xmlChar *) "settings");
    xmlDocSetRootElement(doc, settings_node);
    const char **key = fsfuse_config_settings_keys;
    while (*key) {
        const char *value = g_hash_table_lookup(fsfuse_config.settings, *key);
        if (value != NULL) {
            // printf("XMl adding child %s -> %s\n", *key, value);
            xmlNewTextChild(settings_node,
                            NULL,
                            (const xmlChar *) *key,
                            (const xmlChar *) value);
        }
        key++;
    }
#if 1
    xmlBufferPtr buffer = xmlBufferCreate();
    xmlNodeDump(buffer, doc, settings_node, 0, 1);
    printf("%s\n", buffer->content);
    xmlBufferFree(buffer);
#endif
    return doc;
}

// static void fsfuse_config_init_settings(void)
// {
//     const char **key = fsfuse_config_settings_keys;
//     while (*key) {

//     }
// }

// ----------------------------------------------------------------------------

static void load_fuse_overrides(void)
{
    const char **keys = fsemu_option_keys();
    const char **key = keys;
    while (*key) {
        printf("key: %s\n", *key);
        if (g_str_has_prefix(*key, "fuse_")) {
            fuse_setting(*key + 5, fsemu_option_const_string(*key));
        }
        key++;
    }
    fsemu_option_keys_free(keys);
}

void fsfuse_config_init(void)
{
    // Insert placeholders for all valid fuse options so we know which names
    // are valid.
    fsfuse_config.settings =
        g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
    const char **key = fsfuse_config_settings_keys;
    while (*key) {
        g_hash_table_insert(fsfuse_config.settings, strdup(*key), NULL);
        key++;
    }

    fsfuse_config.argv = (char **) malloc(sizeof(void *) * MAX_ARGS);
    // The program name/path is always expected as the first argument.

    char buffer[PATH_MAX];
    fsemu_application_exe_path(buffer, PATH_MAX);
    fsfuse_add_arg(buffer);

    const char *model = fsemu_option_const_string(OPTION_ZXS_MODEL);
    printf("OPTION_ZXS_MODEL = %s\n", model);
    if (model != NULL) {
        if (strcmp(model, "16k") == 0 || strcmp(model, "16") == 0) {
            fuse_setting("machine", "16");
        } else if (strcmp(model, "48") == 0 || strcmp(model, "48k") == 0) {
            fuse_setting("machine", "48");
        } else if (strcmp(model, "128") == 0 || strcmp(model, "128k") == 0) {
            fuse_setting("machine", "128");
        } else if (strcmp(model, "+2") == 0 || strcmp(model, "plus2") == 0) {
            fuse_setting("machine", "plus2");
        } else if (strcmp(model, "+2a") == 0 || strcmp(model, "plus2a") == 0) {
            fuse_setting("machine", "plus2a");
        } else if (strcmp(model, "+3") == 0 || strcmp(model, "plus3") == 0) {
            fuse_setting("machine", "plus3");
        } else {
            // fsfuse_log_warning("Unknown model specified\n");
        }
    }

    if (fsemu_option_disabled(OPTION_TAPE_DRIVE_SOUNDS)) {
        fuse_setting("loadingsound", "0");
        // fsfuse_add_arg("--no-loading-sound");
        // g_hash_table_insert(
        //    fsfuse_config.settings, strdup("loadingsound"), strdup("0"));
    } else {
        fuse_setting("loadingsound", "1");
        // fsfuse_add_arg("--loading-sound");
        // g_hash_table_insert(
        //     fsfuse_config.settings, strdup("loadingsound"), strdup("1"));
    }

    const char *tape_drive_0 = fsemu_option_const_string(OPTION_TAPE_DRIVE_0);
    if (tape_drive_0) {
        // FIXME: If loading a disk image this way, this will automatically
        // select the plus3 model!
        fsfuse_add_arg(tape_drive_0);
    }

    fuse_setting("zxprinter", "0");
    fuse_setting("graphicsfilter", "2x");

    load_fuse_overrides();

    // Do not enable prompt settings
    fuse_setting("diskaskmerge", "0");
}

// ----------------------------------------------------------------------------

// clang-format off
const char *fsfuse_config_settings_keys[] = {
    "accelerateloader",
    "aspecthint",
    "autoload",
    "autosavesettings",
    "beta128",
    "beta12848boot",
    "betadisk",
    "bwtv",
    "cmosz80",
    "competitioncode",
    "competitionmode",
    "compressrzx",
    "confirmactions",
    "covox",
    "debuggercommand",
    "detectloader",
    "didaktik80",
    "didaktik80disk",
    "disciple",
    "discipledisk",
    "diskaskmerge",
    "disktrymerge",
    "divide",
    "dividemasterfile",
    "divideslavefile",
    "dividewriteprotect",
    "divmmc",
    "divmmcfile",
    "divmmcwriteprotect",
    "dock",
    "doublescanmode",
    "doublescreen",
    "drive40maxtrack",
    "drive80maxtrack",
    "drivebeta128atype",
    "drivebeta128btype",
    "drivebeta128ctype",
    "drivebeta128dtype",
    "drivedidaktik80atype",
    "drivedidaktik80btype",
    "drivedisciple1type",
    "drivedisciple2type",
    "driveopus1type",
    "driveopus2type",
    "driveplus3atype",
    "driveplus3btype",
    "driveplusd1type",
    "driveplusd2type",
    "embedsnapshot",
    "fastload",
    "fbmode",
    "fuller",
    "fullscreen",
    "graphicsfile",
    "graphicsfilter",
    "if2cart",
    "interface1",
    "interface2",
    "issue2",
    "joystick1",
    "joystick1fire1",
    "joystick1fire10",
    "joystick1fire11",
    "joystick1fire12",
    "joystick1fire13",
    "joystick1fire14",
    "joystick1fire15",
    "joystick1fire2",
    "joystick1fire3",
    "joystick1fire4",
    "joystick1fire5",
    "joystick1fire6",
    "joystick1fire7",
    "joystick1fire8",
    "joystick1fire9",
    "joystick1output",
    "joystick2",
    "joystick2fire1",
    "joystick2fire10",
    "joystick2fire11",
    "joystick2fire12",
    "joystick2fire13",
    "joystick2fire14",
    "joystick2fire15",
    "joystick2fire2",
    "joystick2fire3",
    "joystick2fire4",
    "joystick2fire5",
    "joystick2fire6",
    "joystick2fire7",
    "joystick2fire8",
    "joystick2fire9",
    "joystick2output",
    "joystickkeyboarddown",
    "joystickkeyboardfire",
    "joystickkeyboardleft",
    "joystickkeyboardoutput",
    "joystickkeyboardright",
    "joystickkeyboardup",
    "joystickprompt",
    "kempston",
    "kempstonmouse",
    "keyboardarrowsshifted",
    "latetimings",
    "loadingsound",
    "machine",
    "mdrlen",
    "mdrrandomlen",
    "melodik",
    "microdrive2file",
    "microdrive3file",
    "microdrive4file",
    "microdrive5file",
    "microdrive6file",
    "microdrive7file",
    "microdrive8file",
    "microdrivefile",
    "mouseswapbuttons",
    "moviecompr",
    "moviestart",
    "moviestopafterrzx",
    "multiface1",
    "multiface128",
    "multiface1stealth",
    "multiface3",
    "opus",
    "opusdisk",
    "paltv2x",
    "phantomtypistmode",
    "playbackfile",
    "plus3detectspeedlock",
    "plus3disk",
    "plusd",
    "plusddisk",
    "printer",
    "rate",
    "rawrs232",
    "rawsnet",
    "recordfile",
    "recreatedspectrum",
    "rom1280",
    "rom1281",
    "rom16",
    "rom48",
    "rombeta128",
    "romdidaktik80",
    "romdisciple",
    "rominterfacei",
    "rommultiface1",
    "rommultiface128",
    "rommultiface3",
    "romopus",
    "rompentagon0",
    "rompentagon1",
    "rompentagon10240",
    "rompentagon10241",
    "rompentagon10242",
    "rompentagon10243",
    "rompentagon2",
    "rompentagon5120",
    "rompentagon5121",
    "rompentagon5122",
    "rompentagon5123",
    "romplus20",
    "romplus21",
    "romplus2a0",
    "romplus2a1",
    "romplus2a2",
    "romplus2a3",
    "romplus30",
    "romplus31",
    "romplus32",
    "romplus33",
    "romplus3e0",
    "romplus3e1",
    "romplus3e2",
    "romplus3e3",
    "romplusd",
    "romscorpion0",
    "romscorpion1",
    "romscorpion2",
    "romscorpion3",
    "romspeccyboot",
    "romspecse0",
    "romspecse1",
    "romtc2048",
    "romtc20680",
    "romtc20681",
    "romts20680",
    "romts20681",
    "romusource",
    "rs232handshake",
    "rs232rx",
    "rs232tx",
    "rzxautosaves",
    "sdlfullscreenmode",
    "separation",
    "simpleide",
    "simpleidemasterfile",
    "simpleideslavefile",
    "slttraps",
    "snapsasz80",
    "snapshot",
    "snet",
    "sound",
    "sounddevice",
    "soundforce8bit",
    "soundfreq",
    "speakertype",
    "speccyboot",
    "speccyboottap",
    "specdrum",
    "spectranet",
    "spectranetdisable",
    "speed",
    "statusbar",
    "strictaspecthint",
    "svgamodes",
    "tapefile",
    "tapetraps",
    "textfile",
    "unittests",
    "usource",
    "volumeay",
    "volumebeeper",
    "volumecovox",
    "volumespecdrum",
    "writableroms",
    "zxatasp",
    "zxataspmasterfile",
    "zxataspslavefile",
    "zxataspupload",
    "zxataspwriteprotect",
    "zxcf",
    "zxcfcffile",
    "zxcfupload",
    "zxmmc",
    "zxmmcfile",
    "zxprinter",
    NULL
};
// clang-format on
