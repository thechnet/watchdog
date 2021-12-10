/*
signals.c - watchdog
Modified 2021-12-10
*/

/* Header-specific includes. */
#include "signals.h"

/* Implementation-specific includes. */
#include <string.h>
#include <signal.h>
#include "reporter.h"
#include "pulse.h"

/*
*** Signals interface.
*/

/*
Register signals.
*/
void wd_signals_register(void)
{
  if (signal(SIGFPE, &wd_signals_handle) == SIG_ERR ||
      signal(SIGILL, &wd_signals_handle) == SIG_ERR ||
      signal(SIGSEGV, &wd_signals_handle) == SIG_ERR) {
    wd_alerts++;
    warn(WD_MSG_SIGNAL);
  }
}

/*
Handle signals.
*/
void wd_signals_handle(int signum)
{
  /* Prefix. */
  #define prefix __LOGGING_STYLE_FAIL __LOGGING_ID
  write(STDOUT_FILENO, prefix, strlen(prefix)+1);
  
  /* Signal type. */
  char *msg;
  switch (signum) {
    case SIGFPE:
      msg = WD_MSG_SIGFPE;
      break;
    case SIGILL:
      msg = WD_MSG_SIGILL;
      break;
    case SIGSEGV:
      msg = WD_MSG_SIGSEGV;
      break;
    default:
      assert(false);
  }
  write(STDOUT_FILENO, msg, strlen(msg)+1);
  
  /* EOL. */
  #define eol RESET "\n"
  write(STDOUT_FILENO, eol, strlen(eol)+1);
  
  /* Pulse. */
  #undef prefix
  #define prefix __LOGGING_STYLE_INFO __LOGGING_ID WD_MSG_PULSE
  write(STDOUT_FILENO, prefix, strlen(prefix)+1);
  write(STDOUT_FILENO, wd_pulse.file, strlen(wd_pulse.file)+1);
  write(STDOUT_FILENO, (char[1]){':'}, 1);
  wd_int_out(wd_pulse.line);
  write(STDOUT_FILENO, (char[1]){'.'}, 1);
  write(STDOUT_FILENO, eol, strlen(eol)+1);
  
  abort();
}
