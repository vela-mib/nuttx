/************************************************************
 * sched_setupidlefiles.c
 *
 *   Copyright (C) 2007 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <spudmonkey@racsa.co.cr>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name Gregory Nutt nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ************************************************************/

/************************************************************
 * Included Files
 ************************************************************/

#include <nuttx/config.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <errno.h>
#include "os_internal.h"

/************************************************************
 * Private Functions
 ************************************************************/

/************************************************************
 * Public Functions
 ************************************************************/

/************************************************************
 * Function:  sched_setupidlefiles
 *
 * Description:
 *   Configure the idle thread's TCB.
 *
 * Parameters:
 *   tcb - tcb of the idle task.
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *
 ************************************************************/

#if CONFIG_NFILE_DESCRIPTORS > 0

int sched_setupidlefiles(FAR _TCB *tcb)
{
  int fd;

  /* Allocate file descriptors for the TCB */

  tcb->filelist = files_alloclist();
  if (!tcb->filelist)
    {
      *get_errno_ptr() = ENOMEM;
      return ERROR;
    }

#ifdef CONFIG_DEV_CONSOLE
  /* Open stdin, dup to get stdout and stderr. */

  fd = open("/dev/console", O_RDWR);
  if (fd == 0)
    {
      (void)dup2(0, 1);
      (void)dup2(0, 2);
    }
  else
    {
      (void)close(fd);
      *get_errno_ptr() = ENFILE;
      return ERROR;
    }

#if CONFIG_NFILE_STREAMS > 0
  /* Allocate file strems for the TCB */

  return sched_setupstreams(tcb);
#else
  return OK;
#endif /* CONFIG_NFILE_STREAMS */
#endif /* CONFIG_DEV_CONSOLE */
}

#endif /* CONFIG_NFILE_DESCRIPTORS */
