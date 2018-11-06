String help;

void helpFeedback() {
  help = "'Juice?' - Check battery level.\n"
         "'Where?' - Retrieve location.\n"
         "'Aware' - Activate aware mode.\n"
         "'Auth [password]' - Authorize new user.";
  smsSender(phoneNumber, help);
}
