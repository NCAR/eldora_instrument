struct MinirimsCommand {
  u_long cmd;
};

struct MinirimsStatus {
  u_long status;
};

program MinirimsControl {
  version MinirimsCtrlVers {
    int sendMinirimsCommand(struct MinirimsCommand) = 1;
    struct MinirimsStatus statusMinirims(void) = 2;
  } = 1;
} = 0x30000300;
