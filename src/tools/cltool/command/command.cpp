#pragma once


namespace ches {
    class Command {
    public:
        static cmdprocs procs;

        std::string cmdName;
        std::unordered_map<std::string, std::string> cmdArgs;

        Command();

        Command(std::vector<std::string> args);

        std::string argKeyAt(std::string key);

        bool existsArgKey(std::string key);

        static Command fromMainArgs(int argc, char *argv[]) {
            std::vector<std::string> args;

            for(int i = 0; i < argc; i++)
                args.push_back(argv[i]);

            return Command(args);
        }

        void run(cmdprocs procs);

        void setDebugMode();

        void setDisplayLimit();
    };
};



