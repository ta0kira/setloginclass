/* -----------------------------------------------------------------------------
Copyright 2014,2020 Kevin P. Barry

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
----------------------------------------------------------------------------- */

// Author: Kevin P. Barry [ta0kira@gmail.com]

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <login_cap.h>


int main(int argc, char *argv[])
{
  int set_mac = 0x00, index = 1;

  if (index < argc && strcmp(argv[index], "-m") == 0)
    {
      set_mac = LOGIN_SETMAC;
      ++index;
    }

  if (argc - index < 1)
    {
      fprintf(stderr, "%s (-m) [class] [command...]\n", argv[0]);
      return 1;
    }

  const char *class_name = argv[index++];

  login_cap_t *class_cap = login_getclassbyname(class_name, NULL);

  if (class_cap && strcmp(class_cap->lc_class, class_name) == 0)
    {
      if (setusercontext(class_cap, NULL, 0, LOGIN_SETRESOURCES | LOGIN_SETPRIORITY | LOGIN_SETUMASK | LOGIN_SETCPUMASK) < 0)
        {
          fprintf(stderr, "%s: unable to set login limits for class '%s': %s\n", argv[0], class_name, strerror(errno));
          login_close(class_cap);
          return 1;
        }

      if (setusercontext(class_cap, NULL, 0, set_mac | LOGIN_SETLOGINCLASS) < 0)
        {
          fprintf(stderr, "%s: unable to set login class to '%s': %s\n", argv[0], class_name, strerror(errno));
          login_close(class_cap);
          return 1;
        }
      login_close(class_cap);
    }

  else
    {
      fprintf(stderr, "%s: unable to set login class to '%s': %s\n", argv[0], class_name,
        class_cap? "login class does not exist" : strerror(errno));
      return 1;
    }

  if (argc - index < 1)
    {
      char *shell = getenv("SHELL");
      if (!shell || !strlen(shell))
	{
	  struct passwd *user = getpwuid(getuid());
	  if (user) shell = user->pw_shell;
	  endpwent();
	}
      if (!shell || !strlen(shell))
	{
	  fprintf(stderr, "%s: no shell specified\n", argv[0]);
	  return 1;
	}

      char *command[] = { shell, NULL };
      execvp(command[0], command);
      fprintf(stderr, "%s: unable to execute '%s': %s\n", argv[0], command[0], strerror(errno));
    }

  else
    {
      execvp(argv[index], argv + index);
      fprintf(stderr, "%s: unable to execute '%s': %s\n", argv[0], argv[2], strerror(errno));
    }

  return 1;
}
