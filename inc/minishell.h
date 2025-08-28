/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:56:38 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/27 10:44:45 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// OWN LIBRARIES
# include "../lib/libft/libft.h"
# include "../lib/gnl/get_next_line.h"

// EXTERNAL LIBRARIES PARSER
# include <unistd.h>	// write getcwd chdir
# include <stdio.h>  	// printf aux
# include <fcntl.h>		// open
# include <stdbool.h>	// bool

# include <readline/readline.h> 	// readline
# include <readline/history.h>	// readline

// EXTERNAL LIBRARIES BUILTINS
# include <stdlib.h>	// exit 

// EXTERNAL LIBRARIES EXECUTOR
# include <sys/wait.h>
# include <unistd.h>
# include <signal.h> // SIGINT

// MACROS EXECUTOR -------------------------------------------------

# define MAX_PID 999 

// MACROS PARSER-----------------------------------------------------

# define FAILURE			-1
# define SUCCESS			0
# define ERROR				1
# define SYNTAX_ERROR		2

# define COMMAND_NOT_FOUND	127
# define EXIT_SIGINT		130		// Ctrl+'C'
# define EXIT_SIGQUIT       131    	// Ctrl+'\'

# define ERROR_ENVIRONMENT "Error\nEnvironment unavailable or empty\n"
# define ERROR_OPEN_FILE "Error\nOpening file\n"
# define ERROR_INPUT_READER "Error\nError read input\n"
# define ERROR_CHECK_SYNTAX "Error\nCheck syntax input Error\n"
# define ERROR_ADVANCE_INDEX "Error\nIndex advance error\n"

# define ERROR_QUOTE_SYNTAX "Error\nSyntax error: Unmatched quotes detected\n"
# define ERROR_REDIR_SYNTAX "Error\nSyntax error: Invalid redirection syntax\n"
# define ERROR_PIPE_SYNTAX "Error\nSyntax error: Invalid pipe syntax\n"

# define ERROR_INIT "Error\n Initialization error\n" 
# define ERROR_COMMAND_INIT "Error\n Command structure initialization failed\n"
# define ERROR_WORD_INIT "Error\n Word structure initialization failed\n" 
# define ERROR_TOKEN_INIT "Error\n Token structure initialization failed\n" 
# define ERROR_EXPAND_INIT "Error\n Expand structure initialization failed\n"

# define ERROR_INVALID_INPUT	"Error\n Invalid input parameter\n"
# define ERR_MEM_ALLOC		"Error\n Memory allocation failed\n"

# define ERROR_COMMAND_EMPTY	"Error\n Empty command detected\n"
# define ERROR_FILE_NOT_FOUND	": No such file or directory"
# define ERROR_PERMISSION_DENIED ": Permission denied"
# define ERROR_CLOSING_FILE		": Closing file"

# define PROMPT					"minishell$ "

# define FREE_ALL_SHELL			"Free\n Total cleaning minishell... OK\n"
# define FREE_ITERATION_INPUT	"Free\n Iteration Input...\t\t\t OK\n"
# define FREE_COMMANDS_LIST		"Free\n Commands list...\t\t\t OK\n"
# define FREE_WORDS_LIST		"Free\n Words list...\t\t\t\t OK\n"
# define FREE_TOKENS_LIST		"Free\n Tokens List...\t\t\t\t OK\n"
# define FREE_EXPANDS_LIST		"Free\n Expands List...\t\t\t OK\n"
# define FREE_MATRIX			"Free\n Cleaning matrix...\t\t\t OK\n"

# define WORD					'W'
# define OUTFILE				'O'
# define APPEND					'A'
# define INFILE					'I'
# define HERE_DOC				'H'
# define PIPE					'P'

# define NO_QUOTES				1
# define SINGLE_QUOTES			2
# define DOUBLE_QUOTES			3
# define OPERATOR				4

# define BASIC_EXPANSION		1
# define CURLY_B				2
# define LAST_EXIT_STATUS		3
# define LITERAL				4

// STRUCT -----------------------------------------------------

// GLOBAL VARIABLE
extern int	g_signal_flag;

// VARIABLES EXPANDIDAS
typedef struct s_expand
{
	int				type;
	int				first_i;
	int				last_index;
	char			*substitution_str;
	char			*key;
	char			*value;
	struct s_expand	*next;
}			t_expand;

// TOKEN
typedef struct s_token
{
	int				type;
	char			*raw_tkn;
	char			*exp_token;
	char			*noquotes_token;
	t_expand		*exp_list;
	struct s_token	*next;
}					t_token;

typedef struct s_data_token
{
	char			*raw_w;
	int				start_index;
	int				token_len;
	int				token_type;
}					t_data_token;

// WORD TOKENS
typedef struct s_word
{
	char			word_type;
	char			*raw_w;
	char			*processed_word;
	t_token			*tokens_list;
	struct s_word	*next;
}					t_word;

typedef struct s_data_word
{
	char			*cmd_input;
	int				start_index;
	int				w_len;
	int				w_type;
}					t_data_word;

typedef struct s_cmd
{
	char			*command;
	char			**args;
	char			*infile;
	char			*outfile;
	char			*delimiter;
	bool			append;
	bool			hd;
	bool			is_btn;
	int				exit_status;
	t_word			*words_list;
	struct s_cmd	*next;
}					t_cmd;

// SHELL
typedef struct s_shell
{
	char		*input;
	char		**environment;
	int			exit_status;
	int			last_exit_status;
	char		**readonly_vars;
	t_cmd		*commands_list;
}				t_shell;

// FUNCTIONS ------------------------------------------

//SRC------------------------------------------------
// 00_main.c 
int			main(int argc, char **argv, char **environment);
void		cleanup_minishell(t_shell *shell);

// 01_execute_shell.c
void		execute_shell(t_shell *shell);
void		process_commands(t_shell *shell);

// END SRC----------------------------------------------

/// PARSER -------------------------------------------
// 01.1_check_syntax.c
int			validate_syntax(t_shell *shell);

// 02_command_generate.c
void		create_commands_structure(t_shell *shell);

// 02.1_command_check.c
int			validate_command_structure(t_shell *shell);

// 02.2_command_builder.c
void		add_command_node(t_cmd **commands_list, char *input);

// 03_lexical_analyzer.c
void		lexical_analyzer(t_cmd *current_command, t_shell *shell);

// 03.1_word_builder.c
int			create_word(t_word **word_list, t_data_word *d_word);
t_word		*find_word_last_node(t_word *word_list);

// 04_tokenizer.c
void		tokenizer(t_word *words_list, t_shell *shell);

// 04.1_token_builder.c
int			create_token(t_token **token_list, t_data_token *data_token);
int			not_is_special_tkn(t_data_token	*d_tkn, int i);
void		print_tokens_list(t_token *token_list);

//  05_variable_expander.c
void		variable_expander(t_word *words_list, t_shell *shell);

//  05.1_expand_list.c
int			basic_expander(t_token *token, int first_index);
int			last_exit_status_expander(t_token *token, int first_index);
int			curly_braces_expander(t_token *token, int first_i, int final_i,
				bool success);
int			literal_expander(t_token *token, int first_index, bool success);

//  05.2_expand_extractor.c
char		*extract_key(char *token, int first_index);
char		*get_environment_var(char **env, char *variable);

//	05.3_expand_builder.c
void		print_expand_nodes_list(t_expand *expand_list);
int			insert_expand_node_value(t_token *token, int last_pos);
t_expand	*add_expand_node(t_expand **expand_list, char *subst_var,
				int first_index, int expand_type);

//  05.4_dequotizer.c
void		dequotize_tokens(t_word *words_list, t_shell *shell);
void		update_quote_state(char character, bool *in_single_quotes,
				bool *in_double_quotes);

// 06_word_processor.c
void		generate_processed_word(t_word *words_list, t_shell *shell);

// 07_semantic_check.c
int			validate_command_semantics(t_shell *shell);

// 08_execution_builder.c 
void		build_execution_structure(t_cmd *commands_list, t_shell *shell);

// 08.1_execution_redict.c
int			process_infile(t_cmd *command, t_word *word);
int			process_outfile(t_cmd *command, t_word *word);
int			process_append(t_cmd *command, t_word *word);
int			process_heredoc(t_cmd *command, t_word *word);

// 09_free_manager.c
void		free_iteration_input(t_shell *shell);
void		free_commands_list(t_cmd **commands_list);
void		free_words_list(t_word **words_list);
void		free_tokens_list(t_token **token_list);
void		free_expands_list(t_expand **expands_list);

// 10_utils_core.c
int			find_pipe_outside_quotes(char *input, int start_index);
int			find_redirection_outside_quotes(char *input, int start_index);
int			find_word_end_outside_quotes(char *input, int start_index);
int			find_next_expansion_outside_single_quotes(const char *input,
				int start_index);
int			find_matching_quote_position(const char *input,
				int quote_start_index);

// 10.1_utils_strings.c
int			advance_index_by_length(int current_index, int length);
char		*ft_strjoin_free(char *str1, char *str2);
void		free_matrix(char **matrix);
int			is_expansion_char(char character);
int			is_space(char character);

// 10.2_utils_basic.c
int			get_operator_length(char *input, int index);
int			is_word_delimiter(char character);
int			is_redirection(char character);
int			is_pipe(char character);
int			is_quote(char character);

// ------------------------------------
// END PARSER--------------------------

// EXECUTOR --------------------------

// 00_utils_clean.c
void		ft_free_cmd_args(t_cmd *cmd);
void		ft_free_cmd_files(t_cmd *cmd);

// 00_utils.c
void		ft_exit_error(char *error);
void		*safe_malloc(size_t bytes);
char		**ft_copy_env(char **env);
char		*ft_substr_malloc(const char *input, int start, int len);
void		ft_shellevel(t_shell *shell);

// ----------------------------------
// 01_signals.c
void		setup_signals(void); // src/01_execute_shell.c
void		ft_handle_sig_quit(int signum);
// 02.00_executer.c
void		exec_commands(t_shell *ms); // src/01_execute_shell.c

// 02.01_executer_command.c
void		execute_command(t_shell *shell, t_cmd *cmd); // 02.00_executer.c

// 03_redirections.c
int			redirections(t_shell *shell, t_cmd *cmd); // 02.00_executer.c

// 03.01_heredoc.c
char		*expand_heredoc(char *buffer, char **env, int exit_st); // 03_redi

// BUILTINS-------------------------------------------------------
//00_exec_builtins.c
void		exec_builtins(t_shell *shell, t_cmd *cmd, int prev_fd);
//00_utils_builtins_00.c
int			ft_mtrx_size(char **mtrx); // unset, export
int			ft_search_index_env(char **env, char *str); // export, unset 
int			ft_valid_env_var(char *env_var); // export, unset
char		**ft_copy_mtrx(char **mtrx); // export,
void		ft_swap_mtrx(char **s1, char **s2); // export
//00_utils_builtins_01.c
int			ft_strcmp(char *s1, char *s2);
char		*ft_find_plus_pos(char *var); // export
void		sort_alphabetic_mtrx(char **mtrx); // export
char		*ft_get_keyvar(char *var); // export
//00_utils_builtins_02.c
char		**ft_create_new_env(char **env, char *var); // export
//00_utils_builtins_03.c
char		**ft_append_env_var_value(char **env, char *var, int index);//export
// 01_exec_echo.c
int			exec_echo(t_cmd *cmd);
// 02_exec_cd.c
int			exec_cd(t_cmd *cmd, t_shell *shell); //
// 03_exec_pwd.c
int			exec_pwd(void);
// 04_exec_export.c
int			exec_export(t_cmd *cmd, t_shell *shell);
char		**ft_add_modify_env(char **env, char *var, int flag);
// 05_exec_unset.c
int			exec_unset(t_shell *shell, t_cmd *cmd);
// 06_exec_env.c
int			exec_env(t_shell *shell);
// 07_exec_exit.c
void		exec_exit(t_shell *shell, t_cmd *cmd, int prev_fd);
// END BUILTINS-------------------------------------------------------
#endif