#include <getopt.h>

#include <ipnoise-common/log.h>
#include "objects/client/handlerTelnet/command/clientHandlerTelnetCommandDelGroup.h"
#include "objects/client/handlerTelnet/clientHandlerTelnetObject.h"

ClientHandlerTelnetCommandDelGroup::ClientHandlerTelnetCommandDelGroup(
    ClientHandlerTelnetObject *handler)
    :   ClientHandlerTelnetCommand(handler)
{
    _handler = handler;
};

ClientHandlerTelnetCommandDelGroup::~ClientHandlerTelnetCommandDelGroup()
{
};

const char *ClientHandlerTelnetCommandDelGroup::getName()
{
    return "api_delGroup";
};

int ClientHandlerTelnetCommandDelGroup::run(int argc, char* const* argv)
{
    PDEBUG(5, "delgroup command received\n");
    char buffer[1024]   = { 0x00 };
    int show_xml        = 0;
    int show_usage      = 0;
    int show_prototype  = 0;
    int send_command    = 0;
    int res, err        = 0;
    string command      = "";

    PLACEHOLDERS::iterator placeholders_i;
    PLACEHOLDERS           placeholders;

    // init placeholders
    placeholders["sessid"]      =   _handler->apiGetSessionId();
    placeholders["id"]          =   "100";
    placeholders["strategy"]    =   "0";

    // init command prototype
    res = do_init(argv[0],
        "<ipnoise ver=\"0.01\" sessid=\"[[sessid]]\">\n"
        "    <commands>\n"
        "       <command type=\"delGroup\" ver=\"0.01\">\n"
        "           <group id=\"[[id]]\" strategy=\"[[strategy]]\"/>\n"
        "       </command>\n"
        "    </commands>\n"
        "</ipnoise>\n"
    );
    if (res){
        err = res;
        goto fail;
    }

    // get args
    while(1) {
        int option_index = 0;
        static struct option long_options[] = {
            {"help",                0, 0, 'h'},
            {"prototype",           0, 0, 'p'},
            {"xml",                 0, 0, 'x'},
            {"sessid",              1, 0, '0'},
            {"id",                  1, 0, '1'},
            {"strategy",            1, 0, '2'},
            {"send",                0, 0, 's'},
            {0, 0, 0, 0}
        };

        int option = getopt_long (argc, argv,
            "hxp0:1:2:s",
            long_options, &option_index
        );

        if (option == -1){
            break;
        }

        switch (option) {
            case 'h':
                show_usage = 1;
                break;

            case 'x':
                show_xml = 1;
                break;

            case 'p':
                show_prototype = 1;
                break;

            case '0':
                placeholders["sessid"] = optarg;
                break;

            case '1':
                placeholders["id"] = optarg;
                break;

            case '2':
                placeholders["strategy"] = optarg;
                break;

            case 's':
                send_command = 1;
                break;
        }
    }

    // check another args
    if (optind < argc){
        snprintf(buffer, sizeof(buffer),
            "Warning: argument(s) will be ignored: ");
        _handler->write(buffer);
        while (optind < argc){
            snprintf(buffer, sizeof(buffer), "'%s' ", argv[optind++]);
            _handler->write(buffer);
        }
        snprintf(buffer, sizeof(buffer), "\n");
        _handler->write(buffer);
    }

    if (show_usage){
        snprintf(buffer, sizeof(buffer),
            "Usage:     %s [options]                                        \n"
            "options:                                                       \n"
            "         -h, --help            This help                       \n"
            "         -p, --xml             Just show prototype             \n"
            "         -x, --xml             Just show xml, don't send       \n"
            "             --sessid          API session ID                  \n"
            "             --id              Group ID for delete             \n"
            "             --strategy        Strategy for delete             \n"
            "         -s, --send            Send command                    \n"
            "\n",
            argv[0]
        );
        _handler->write(buffer);
        goto out;
    }

    if (show_prototype){
        command = getCommand();
        _handler->write("Prototype:\n");
        _handler->write(command);
    }

    // replace placeholders
    command = getCommand(placeholders);

    if (show_xml){
        _handler->write(command);
    }

    if (send_command){
        _handler->do_http_command(command);
    }

out:
    return err;

fail:
    if (!err){
        err = -1;
    }
    goto out;

};

