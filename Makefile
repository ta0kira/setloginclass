# ------------------------------------------------------------------------------
# Copyright 2014,2020 Kevin P. Barry
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ------------------------------------------------------------------------------

# Author: Kevin P. Barry [ta0kira@gmail.com]

NAME=setloginclass
#NOTE: installed in /usr/sbin so that it can be used before /usr/local is mounted
INSTALL=/usr/sbin/$(NAME)
.PHONY: default install clean

default: $(NAME)

$(NAME): setloginclass.c Makefile
	cc -std=c99 -Wall -g -O2 setloginclass.c -o $(NAME) -lutil

install: $(INSTALL)

$(INSTALL): $(NAME)
	install -v -m 700 -o 0 -g 0 $(NAME) $(INSTALL)

clean:
	rm -fv setloginclass *~
