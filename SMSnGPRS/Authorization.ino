void authSetup() {
  smsSender(adminPhoneNumber, "New access to PackVRESTO.");
  memcpy(authPhoneNumber, phoneNumber, strlen(phoneNumber)+1);
  smsSender(authPhoneNumber, "Access granted. Send 'help' to see available commands.");
}
