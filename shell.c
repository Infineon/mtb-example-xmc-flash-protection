/******************************************************************************
 * File Name:   shell.c
 *
 * Description: This is the source code for the XMC MCU: Flash Protection
 *              Example for ModusToolbox. This file includes the shell
 *              command definitions used to process user serial terminal
 *              input.
 *
 * Related Document: See README.md
 *
 ******************************************************************************
 *
 * Copyright (c) 2015-2024, Infineon Technologies AG
 * All rights reserved.
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include "shell.h"
#include "cy_retarget_io.h"
#include "ring_buffer.h"
#include "cybsp.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define SHELL_ERR_SYNTAX   ("Error: Invalid syntax for: %s")
#define SHELL_ERR_CMD      ("Error: No such command: %s")

#define SHELL_BACKSPACE    ((char)(0x08))  /* Backspace. */
#define SHELL_DELETE       ((char)(0x7F))  /* Delete. */
#define SHELL_CTRLC        ((char)(0x03))  /* Ctrl + C. */
#define SHELL_CR           ((char)(0x0D))  /* CR. */
#define SHELL_LF           ((char)(0x0A))  /* LF. */
#define SHELL_ESC          ((char)(0x1B))  /* Esc. */
#define SHELL_SPACE        ((char)(0x20))  /* Space. */

#define SHELL_PROMPT       ">> "
#define SHELL_CMDLINE_SIZE 256
#define SHELL_ARGS_MAX     16

/*******************************************************************************
* Typedefs
*******************************************************************************/
typedef enum
{
    SHELL_STATE_INIT,              /* The Shell service is not initialized. */
    SHELL_STATE_GET_USER_INPUT,    /* The Shell service is accepting user commands. */
    SHELL_STATE_EXEC_CMD,          /* The Shell service is executing user commands. */
    SHELL_STATE_END_CMD            /* The Shell service finished command execution. */
} shell_state_t;

/*******************************************************************************
* Static Variables
*******************************************************************************/
static const shell_cmd_t *shell_cmd_table;
static shell_state_t shell_state;
static uint32_t shell_cmdline_pos;
static char shell_cmdline[SHELL_CMDLINE_SIZE];

/*******************************************************************************
* Function Name: shell_print
********************************************************************************
* Summary:
* Print the string to the serial terminal
*
* Parameters:
*  const char *format - String to be printed
*  ...                - variadic arguments
*
* Return:
*  void
*
*******************************************************************************/
void shell_print(const char *format, ... )
{
    va_list ap;

    va_start(ap, format);
    vprintf(format, ap);

    /* Add new line */
    printf("\r\n");

    va_end(ap);
}

/*******************************************************************************
* Function Name: print_shell_prompt
********************************************************************************
* Summary:
* Prints SHELL> on terminal.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void print_shell_prompt(void)
{
    char prompt[] = SHELL_PROMPT;
    uint8_t i;
    
    for (i=0;i<sizeof(prompt);i++)
    {
        XMC_UART_CH_Transmit(cy_retarget_io_uart_obj.channel,prompt[i]);
    }
}

/*******************************************************************************
* Function Name: shell_make_argv
********************************************************************************
* Summary:
* Returns the number of arguments
*
* Parameters:
*  char *cmdline - shell input string
*  char *argv[]  - pointer to command arguments array
*
* Return:
*  int32_t
*
*******************************************************************************/
static int32_t shell_make_argv(char *cmdline, char *argv[])
{
    int32_t argc = 0;
    int32_t i;
    bool in_text_flag = false;

    if ((cmdline != NULL) && (argv != NULL))
    {
        for (i = 0u; cmdline[i] != '\0'; ++i)
        {
            if (cmdline[i] == ' ')
            {
                in_text_flag = false;
                cmdline[i] = '\0';
            }
            else
            {
                if (argc < SHELL_ARGS_MAX)
                {
                    if (in_text_flag == false)
                    {
                        in_text_flag = true;
                        argv[argc] = &cmdline[i];
                        argc++;
                    }
                }
                /* Return argc.*/
                else
                {
                    break;
                }
            }
        }

        argv[argc] = 0;
    }
    return argc;
}

/*******************************************************************************
* Function Name: shell_process_input
********************************************************************************
* Summary:
* Processes the user input and calls the respective functions
*
* Parameters:
*  None
*
* Return:
*  void
*
*******************************************************************************/
void shell_process_input(void)
{
  /* One extra for 0 terminator.*/
  char *argv[SHELL_ARGS_MAX + 1u];
  int32_t argc;
  uint8_t buf;
    switch (shell_state)
    {
        case SHELL_STATE_INIT:
        {
            print_shell_prompt();
            shell_state = SHELL_STATE_GET_USER_INPUT;
            break;
        }
        case SHELL_STATE_GET_USER_INPUT:
        {
            if (ring_buffer_avail(&serial_buffer) > 0)
            {
                ring_buffer_get(&serial_buffer, &buf);
                if (buf != 0x0D)
                {
                /* Check if
                 * 1. enter was pressed or
                 * 2. shell_cmdline buffer has only 1 character left, reserved for zero termination
                 */
                    if (((char)buf != SHELL_LF) && (shell_cmdline_pos < (SHELL_CMDLINE_SIZE - 1)))
                    {
                        switch(buf)
                        {
                            case SHELL_BACKSPACE:
                            case SHELL_DELETE:
                            {
                                if (shell_cmdline_pos > 0U)
                                {
                                  shell_cmdline_pos -= 1U;
                                  XMC_UART_CH_Transmit(cy_retarget_io_uart_obj.channel,SHELL_BACKSPACE);
                                  XMC_UART_CH_Transmit(cy_retarget_io_uart_obj.channel,' ');
                                  XMC_UART_CH_Transmit(cy_retarget_io_uart_obj.channel,SHELL_BACKSPACE);
                                }
                                break;
                            }
                            default:
                            {
                                if ((shell_cmdline_pos + 1U) < SHELL_CMDLINE_SIZE)
                                {
                                  /* Only printable characters. */
                                    if (((char)buf >= SHELL_SPACE) && ((char)buf <= SHELL_DELETE))
                                    {
                                        shell_cmdline[shell_cmdline_pos] = (char)buf;
                                        shell_cmdline_pos++;
                                        XMC_UART_CH_Transmit(cy_retarget_io_uart_obj.channel,(char)buf);
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
                else
                {
                    /* Append zero termination to command and start execution */
                    shell_cmdline[shell_cmdline_pos] = '\0';
                    XMC_UART_CH_Transmit(cy_retarget_io_uart_obj.channel,SHELL_CR);
                    XMC_UART_CH_Transmit(cy_retarget_io_uart_obj.channel,SHELL_LF);
                    shell_state = SHELL_STATE_EXEC_CMD;
                }
            }
            break;
        }
        case SHELL_STATE_EXEC_CMD:
        {
            argc = shell_make_argv(shell_cmdline, argv);

            if (argc != 0)
            {
                const shell_cmd_t *cur_command = shell_cmd_table;
                while (cur_command->name)
                {
                    /* Command is found. */
                    if (strcasecmp(cur_command->name, argv[0]) == 0)
                    {
                        if (((argc - 1) >= cur_command->min_args) && ((argc - 1) <= cur_command->max_args))
                        {
                            if (cur_command->function)
                            {
                                ((void(*)(int32_t cmd_ptr_argc, char **cmd_ptr_argv))(cur_command->function))(argc, argv);
                            }
                        }
                        else
                        {
                            /* Wrong command syntax. */
                            shell_print(SHELL_ERR_SYNTAX, argv[0]);
                        }
                        break;
                    }
                    cur_command++;
                }
                if (cur_command->name == 0)
                {
                    shell_print(SHELL_ERR_CMD, argv[0]);
                }
            }
            shell_state = SHELL_STATE_END_CMD;
            break;
        }

        case SHELL_STATE_END_CMD:
        {
            shell_state = SHELL_STATE_INIT;
            shell_cmdline_pos = 0u;
            shell_cmdline[0] = 0u;
            break;
        }

        default:
            break;

    }
}

/*******************************************************************************
* Function Name: shell_help
********************************************************************************
* Summary:
* Displays all the commands and their descriptions supported by the shell
*
* Parameters:
*  None
*
* Return:
*  void
*
*******************************************************************************/
void shell_help(void)
{
    const shell_cmd_t *cur_command = shell_cmd_table;

    while (cur_command->name)
    {
        shell_print(">%10s %-24s- %s", cur_command->name,
                cur_command->syntax,
                cur_command->description);
        cur_command++;
    }
}

/*******************************************************************************
* Function Name: shell_init
********************************************************************************
* Summary:
* Initializes the shell with the supported command table and calls provided
* function.
*
* Parameters:
*  const shell_cmd_t *const cmd_table - Pointer to command table structure
*  void (*init) (void)                - Function to execute after initialization
*
* Return:
*  void
*
*******************************************************************************/
void shell_init(const shell_cmd_t *const cmd_table, void (*init)(void))
{
    shell_state = SHELL_STATE_INIT;
    shell_cmdline_pos = 0u;
    shell_cmdline[0] = 0u;
    shell_cmd_table = cmd_table;
    init();
}
/* [] END OF FILE */
