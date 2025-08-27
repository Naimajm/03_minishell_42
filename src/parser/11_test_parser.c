/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   11_test_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 12:28:30 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/05 00:02:44 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <string.h>  // strok

void test_basic_parser(t_shell *shell);

void test_complex_parser(t_shell *shell);
static char **get_bash_args_real(char *input);
static bool is_expected_syntax_error(char *input);
static bool compare_args_arrays(char **minishell_args, char **bash_args);
static void print_args_array(char **args, const char *label);

// Casos de prueba enfocados en argumentos
char *test_cases[] = {
	// ECHO TESTS (básicos primero)
	"echo",
	"echo hello",
	"echo hello world",
	"echo hola mundo",
	"echo """,
	"echo "" hola",
	"echo -n hola",
	"echo -n -n hola",
	"echo -n -nh -n hola",
	"echo -nnnnnnnnn hola",
	"echo $?hcudshfuewrf ew  wr 4ew""""",

	// ECHO TESTS - Casos complejos con comillas mixtas
	"echo \"\"uhjkhnkj\"hjbmb\"'''' defdjhfkjb 					\"iujiouhj\"f'f'f'f'f''f'f'f'f'f'f'",
	"echo \"\"uhjkhnkj\"hjbmb\"'''' defdjhfkjb \"iujiouhj\"f'f'a'j'o''i'w'q''q",
	"echo \"\"aaaaaaaa\"bbbbb\"'''' ccccccccc \"dddddddd\"e'e'e",

	// ECHO TESTS - Casos con variables de entorno
	"echo \"ho   $USER\"  'la   $LESS'   $USER$TERM   ",
	"echo \"ho   $USER\"'la   $LESS'$USER$TERM",			
	// ECHO TESTS - Casos sin espacios
	"echo\"ho   $USER\"",
	"echo\"ho   $USE$LESS\"",

	// ECHO TESTS - Comando entre comillas
	"\"echo ho   $USER\"",
	"\"echo\" \"ho   $USER\"",
	"e\"cho\" \"ho   $USER\"",

	// ECHO TESTS - Casos adicionales con variables
	"echo \"ho   $USER\"",
	"echo \"$HOME '$LESS' $USER\"",
	"echo '$HOME \"$LESS\" $USER'",						
	"echo \"$HOME '$LESS \"pa $TERM\" na' $USER\"",		
	"echo '$HOME \"$LESS 'pa $TERM' na\" $USER'",		
	"echo '$'\"HOME\"",

	// COMANDOS BÁSICOS
	"echo ls -la /home",
	"echo cat file.txt",
	"echo grep pattern file.txt",
	"echo pwd",
	"echo env",

	// EXPORT TESTS - Casos básicos
	"echo export a=b",
	"echo export b=wawa", 
	"echo export b=lala",
	"echo export c=lala=kaka",
	"echo export a",
	"echo export a=",
	"echo export a= b=o",
	"echo export a===",
	"echo export a3=l",
	"echo export 2a=lala",
	"echo export !a=ma",
	
	// EXPORT TESTS - Con variables de entorno
	"echo export $USER=la",
	"echo export $USER=$TERM",
	"echo export $USER=$TERM$HOME",
	"echo export $USER=$TERM$HOM",
	"echo export $HOME=$USER",
	//"echo export $USER$SHLVL",  // esta ok. cambia el valor numero de  $SHLVL
	
	// EXPORT TESTS - Casos especiales
	"echo export",
	"           echo export",
	//"echo export \"\"",			// OK = BENITEZ		// esta ok. bash no genera ""
	"echo export a b=$LESS $USER $HOME=la $COLORTERM=pa c=d",

	// CD TESTS
	"echo cd /home",                    // directorio existe
	"echo cd /nonexistent",             // directorio no existe  
	"echo cd /etc/passwd",              // file existe
	"echo cd",                          // cd solo
	"           echo cd",               // cd con espacios
	"echo cd -",                        // cd - = $OLDPWD
	"echo cd $HOME",                    // cd $HOME (si $HOME no existe)
	"echo cd /root",                    // directorio con distintos permisos
	
	// ENV TESTS
	"echo env",                         // env solo
	"echo env lalal",                   // env con argumento inválido
	"echo env /home",                   // env con directorio
	
	// PWD TESTS
	"echo pwd",                         // pwd sin argumentos
	"echo pwd arg1",                    // pwd con argumentos (inválido)
	"echo pwd arg1 arg2",               // pwd con múltiples argumentos
	
	// UNSET TESTS
	"echo unset",                       // unset sin argumentos
	"echo unset PATH",                  // unset con un argumento
	"echo unset PATH HOME USER",        // unset con varios argumentos
	"echo unset NONEXISTENT_VAR",       // unset variable inexistente
	
	// EXIT TESTS
	"echo exit",                        // exit sin argumentos
	"echo exit 0",                      // exit con código 0
	"echo  exit 1",                      // exit con código 1
	"echo exit 42",                     // exit con código 42
	"echo exit lalal",                  // exit con argumento inválido
	"echo exit 256",                    // exit con código > 255
	"echo exit -1",                     // exit con código negativo
	
	// PIPE TESTS
	"echo ls wc",                     	// pipe básico
	"echo lswc",                       	// pipe sin espacios
	"echo sort colors.txt uniq sort -r  wc",  // pipe múltiple
	"echo ls  wc",                    	// double pipe (OR lógico)
	"echo ls    wc",                 	// pipe con comando vacío
	"echo ls     wc",         			// pipe con redirecciones mezcladas
	"echo ls  wc ",                   	// pipe terminando en pipe
	"echo ",                           	// solo pipe
	"echo  ls  wc",                   	// pipe al inicio

	// REDIRECTION TESTS - Casos básicos 
	"echo hola  file",                 	// Output redirection básica
	"echo hola file",                 	// Sin espacios
	"echo wc  colors.txt",             	// Input redirection básica
	"echo wc  colors.txt  file",      	// Input y output combinadas
	"echo cat colors.txt  wc",         	// Pipe simple (referencia)
	
	// REDIRECTION TESTS - Casos múltiples
	"echo hola  file  file2  file3",    // Múltiples outputs
	"echo hola  file  file2  file3  ls", // Múltiples outputs + pipe
	
	// REDIRECTION TESTS - Heredoc básicos
	"echo cat  EOF",                  	// Heredoc simple
	"echo hola  file  EOF",             // Output + heredoc
	"echo cat  EOF  file",          	// Heredoc + output
	"echo cat  EOF  ls",           		// Heredoc + pipe
	
	// REDIRECTION TESTS - Heredoc múltiples
	"echo cat  hola  que  tal",       	// Múltiples heredoc

	// REDIRECTION TESTS - Pipes complejos
	"echo hola  cat  cat  cat  cat  cat  cat  cat", // Pipe muy largo	

	// INPUTS PERSONALIZADOS SIN OPERADORES	
	//"echo \"cat  ho\"la a'$e\" ta'l   ,qu'e\" hac\"$\"es ",	
	"echo 'ho<<$la' 'cara<>|cola'  'ey '  ",
	"  \"ho<< $? la\" \"car$a<>|cola\"  ' ey'  ", 
	" h\"ola $HOME \"caracola  ",	
	" \"aa $HOME $? bb ${HOME}xxx zz\" ",	

	// CASOS ESPECIALES
	" \" $ \"",
	"'$ '",
	"\"$  $HOME\"",	
	"\" $ $\"",				
	"'$?'",
	"$ \" $ \"",
	"\"$ $ \"",
	"\"$ $\"",	
	"\"$ff{HOME}ff\"",
	"\"ff${HOME}ff\"",
	"xx\"aaa$ezzz\"xx",

	// 2 VARIABLES CONSECUTIVAS
	"$HOME$SHELL", 
	"\"$HOME$SHELL\"",
	"\"zz$HOME$SHELL  xx\"",
	"$SHELL$TERM",
	"$SHELL$XXXX",
	"$HOME=la",

	// LITERALES
	"cc\"\\$literal\"",
	//"\" aa\"\\$a\\$b\"xx \"",	// error
	//"dd\"\\\\$USER\"",		// error	
	//"dd\"\\$USER\"",			// error
	//"ee\"\\\"\\$test\\\"\"",	// error
	"aa\"$HOME\"zz   bb\"$?\"yy   cc\"\\$literal\"  \"${HOME}xxx\"ww",
	"'aa $HOME bb \\$literal ${HOME}xxx $?z zz'",
	" aa\"\\$a\\$b\"xx ",	

	/// CHECK SYNTAX --------------------------------------------------------

	/// PIPE TESTS - ERRORES DE SINTAXIS
	// 1. PIPES AL INICIO (error de sintaxis)
	"| echo hello",                          // Pipe al inicio
	"|echo hello",                           // Pipe al inicio sin espacio
	"  | echo hello",                        // Pipe al inicio con espacios
	"	| echo hello",                       // Pipe al inicio con tab

	// 2. PIPES AL FINAL (error de sintaxis)  
	"echo hello |",                          // Pipe al final
	"echo hello |  ",                        // Pipe al final con múltiples espacios
	"echo hello |	",                       // Pipe al final con tab

	// 3. PIPES MÚLTIPLES CONSECUTIVOS (error de sintaxis)
	"echo hello || echo world",              // Double pipe (OR lógico - no soportado)
	"echo hello ||| echo world",             // Triple pipe (error)
	"echo hello ||||",                       // Múltiples pipes al final
	"|| echo hello",                         // Double pipe al inicio
	"echo hello | | echo world", 		     // Pipes separados por espacio
	"echo hello |  | echo world",    	     // Pipes separados por múltiples espacios

	// 5. PIPES DENTRO DE PALABRAS/TOKENS (casos especiales)
	//"echo|hello",                 	// error         	// Pipe pegado sin espacios
	//"echo|",                          // OK = BENITEZ 	// Comando pegado a pipe al final
	//"|hello",                     	// OK = BENITEZ    	// Pipe pegado al inicio sin espacios
	//"ech|o hello",                    // OK = BENITEZ      // Pipe en medio de palabra
	//"echo hel|lo",                  	// OK = BENITEZ        // Pipe en medio de argumento
	"echo 'hello|world'",               // Pipe dentro de comillas simples (válido)
	"echo \"hello|world\"",             // Pipe dentro de comillas dobles (válido)

	// 6. PIPES CON COMANDOS VACÍOS
	"echo hello |  | echo world",    				// Comando vacío entre pipes
	"echo hello |   |   | echo world",       		// Múltiples comandos vacíos
	" | | ",                                 		// Solo pipes y espacios
	"||",                                    		// Solo double pipe
	"| |",                                   		// Pipes separados por espacio

	// 7. PIPES CON HEREDOC (casos complejos)
	"cat << EOF | ",                    // Heredoc seguido de pipe al final
	"| cat << EOF",                     // Pipe al inicio con heredoc
	"cat << EOF | | cat",               // Heredoc con pipes múltiples
	//"echo hello | << EOF",            // DISTINTO BENITEZ        // Pipe seguido de heredoc sin comando

	// 8. CASOS EXTREMOS Y COMBINACIONES
	//"echo | | | hello",      			// OK = BENITEZ		// Múltiples pipes con argumentos mezclados
	"| | echo hello | |",               // OK = BENITEZ		// Pipes al inicio y al final
	"echo 'hello | world' |",  			// OK = BENITEZ  	// Comillas con pipe interno + pipe real al final
	"echo \"hello | world\" | | echo test", 				// Comillas con pipe interno + error de sintaxis
	//"echo$USER|echo$HOME",            // OK = BENITEZ     // Variables pegadas a pipes
	//"$USER|$HOME",                   	// OK = BENITEZ    	// Solo variables con pipe
	"|$USER",                                				// Pipe + variable al inicio
	"$USER|",                        	// OK = BENITEZ     // Variable + pipe al final

	// 9. PIPES CON ESPACIOS ESPECIALES
	"echo hello |		",              // Pipe con tab al final
	//"	|	echo hello",               	// OK = BENITEZ         // Tabs alrededor de pipe inicial
	//"echo hello	|	echo world",    // DISTINTO BENITEZ    // Pipe con tabs alrededor
	//"echo hello \n| echo world",      // Pipe con caracteres especiales

	// CASOS VÁLIDOS PARA COMPARAR (deberían funcionar)
	"echo hello | cat",                     // Pipe básico válido
	//"echo hello | cat | wc",           	// OK = BENITEZ      // Pipe múltiple válido
	"echo 'pipe|inside' | cat",             // Pipe literal dentro de comillas
	"echo \"pipe|inside\" | cat",           // Pipe literal dentro de comillas dobles

	// CASOS EDGE CON VARIABLES Y PIPES
	//"echo $USER | echo $HOME",          // OK = BENITEZ   // Variables con pipe válido
	//"$USER|$HOME",                      // OK = BENITEZ   // Variables con pipe sin espacios (error)
	//"echo $USER| echo $HOME",           // OK = BENITEZ     // Variable pegada a pipe (error)
	//"echo $USER |echo $HOME",           // OK = BENITEZ    // Comando pegado después de pipe (error)

	// 10. CASOS MIXTOS CON OPERADORES
	//"echo hello | > file < input",  	// DISTINTO BENITEZ         // Pipe con múltiples redirecciones
	//"echo hello | >> file << EOF",    // DISTINTO BENITEZ    // Pipe con append y heredoc
	"echo hello || > file",                  // Double pipe con redirección
	//"echo hello | & echo world",    	// OK = BENITEZ    // Pipe con background operator

	// 4. PIPES MEZCLADOS CON REDIRECCIONES (casos complejos)
	//"echo hello | > file",        	// error            // Pipe seguido de redirección sin comando
	//"echo hello > file |",            // OK = BENITEZ     // Redirección seguida de pipe al final
	"echo hello | | > file",          				// Pipes múltiples con redirección
	//"echo hello | < file",            // OK = BENITEZ     	// Pipe seguido de input redirection
	//"< file | echo hello",            // OK = BENITEZ     // Input redirection seguida de pipe al inicio

	// INPUTS PERSONALIZADOS CON OPERADORES
	//"hola < cara | \"$HOME\" >> caracola | \"$HOME\" << heredoc ",
	//"  cmd1 'literal' > O  \"hola x\" 1234 | cmd2 << H  ",
	//"aa > aa >> bb << bb < cc",			// OK = BENITEZ

	/// REDIRECTION TESTS - ERRORES DE SINTAXIS
	"</<</>/>>",                     		// OK = BENITEZ      // Solo operadores (error sintaxis)
	"echo hola >>>>>>> file",              // Múltiples > seguidos (error)
	"echo hola <<<<<<< file",              // Múltiples < seguidos (error)
	"echo hola </<</>/>>   </<</>/>> file", // OK = BENITEZ// Operadores mezclados (error)
	"echo hola </<</>/>>   | file",    // OK = BENITEZ    // Redirección + pipe mezclados (error)
	"echo hola > file </<</>/>>",        // OK = BENITEZ  // Redirección válida + inválida (error)
	//"<< EOF",                              // Heredoc sin comando (error)

	// ERROR SINTAXIS
	//"'$\"' '$",					// error	
	//"export GHOST= 123 | env | grep GHOST",
	//"$EMPTY echo hi",
	NULL
};

void test_basic_parser(t_shell *shell)
{
    int index;
    int test_number;
    int passed;
    int failed;
    int syntax_errors;
    int *failed_tests;
    int failed_count;
	bool test_passed;

    printf("\n🧪 TESTING PARSER - SIMPLIFIED\n");
    printf("===============================\n");    
    printf("📋 Testing basic parsing pipeline only...\n\n");
    
    index 			= 0;
    test_number 	= 0;
    passed 			= 0;
    failed 			= 0;
    syntax_errors	= 0;
    failed_count 	= 0;	
    failed_tests 	= malloc(sizeof(int) * 1000); // Tamaño suficiente para todos los tests

    while (test_cases[index])
    {
        test_number++;
        printf("TEST %d: %s\n", test_number, test_cases[index]);
        
        shell->input = ft_strdup(test_cases[index]);
        test_passed = true;
        
        printf("  0. Syntax validation...");
        if (validate_syntax(shell) == SYNTAX_ERROR)
        {
            if (is_expected_syntax_error(test_cases[index]))
            {
                printf(" ✅ SYNTAX ERROR (expected)\n");
                syntax_errors++;
                passed++; 		// Contar como éxito porque detectó el error correctamente
            }
            else
            {
                printf(" ❌ UNEXPECTED SYNTAX ERROR\n");
                failed++;
                failed_tests[failed_count++] = test_number; // Guardar número de test fallido
            }
            test_passed = false;
        }
        else
            printf(" ✅ OK\n");   
        if (test_passed)	// Solo continuar si no hay errores de sintaxis
        {
            printf("  1. Syntax analyzer...");
			create_commands_structure(shell);
			if (!shell->commands_list)
			{
				printf(" ❌ FAILED\n");
				failed++;
				failed_tests[failed_count++] = test_number;
				test_passed = false;
			}
			else
			{
				if (validate_command_structure(shell) == SYNTAX_ERROR)
				{
					printf(" ❌ FAILED (command structure)\n");
					failed++;
					failed_tests[failed_count++] = test_number;
					test_passed = false;
				}
				else
					printf(" ✅ OK\n");
			}
        }
        if (test_passed)
        {
            printf("  3. Process commands...");
            process_commands(shell);
            if (!shell->commands_list->words_list)
            {
                printf(" ❌ FAILED (processing pipeline)\n");
                failed++;
                failed_tests[failed_count++] = test_number;
                test_passed = false;
            }
            else
            {
                if (validate_command_semantics(shell) == SYNTAX_ERROR)
                {
                    printf(" ❌ FAILED (command semantics)\n");
                    failed++;
                    failed_tests[failed_count++] = test_number;
                    test_passed = false;
                }
                else
                    printf(" ✅ OK\n");
            }
        }
        if (test_passed)
        {
            printf("  4. Build execution...");
            build_execution_structure(shell->commands_list, shell);
            if (!shell->commands_list->args)
            {
                printf(" ❌ FAILED\n");
                failed++;
                failed_tests[failed_count++] = test_number; // Guardar número de test fallido
                test_passed = false;
            }
            else
            {
                printf(" ✅ OK\n");
                print_args_array(shell->commands_list->args, "Generated args");
            }
        }        
        if (test_passed) 				// Solo tests que pasaron todas las fases
            passed++;     
        // Cleanup después de cada test
        free(shell->input);
        if (shell->commands_list)
            free_commands_list(&shell->commands_list);
        shell->input = NULL;
        shell->commands_list = NULL;
        shell->exit_status = 0; 				       
        printf("────────────────────────────────\n\n");
        index++;
    }    
    printf("\n📊 BASIC PARSER TEST SUMMARY\n");
    printf("============================\n");
    printf("✅ Passed: \t\t %d/%d\n", passed, test_number);
    printf("❌ Failed: \t\t %d/%d\n", failed, test_number);
    printf("Success rate: \t %.1f %%\n", (float) passed / (float) test_number * 100);
        
    if (syntax_errors > 0)		// Mostrar desglose de resultados
    {
        printf("\n📝 Test breakdown:\n");
        printf("   • Valid syntax tests: \t %d/%d\n", passed - syntax_errors, test_number - syntax_errors);
        printf("   • Syntax error tests: \t %d (correctly detected)\n", syntax_errors);
    }    
    if (failed == 0)
        printf("🎉 All basic pipeline tests passed!\n");
    else
    {
        printf("⚠️ %d tests need attention: [", failed);
        for (int i = 0; i < failed_count; i++)
        {
            printf("%d", failed_tests[i]);
            if (i < failed_count - 1)
                printf(", ");
        }
        printf("]\n");
    }
    free(failed_tests);
    cleanup_minishell(shell);		
    exit(0);
}

void test_complex_parser(t_shell *shell)
{
    int index;
    int test_number;
    int passed;
    int failed;
    int syntax_errors;
    int *failed_tests;
    int failed_count;
    char **minishell_args;
    char **bash_args;
    
    printf("\n🧪 TESTING PARSER ARGUMENT GENERATION\n");
    printf("=====================================\n");
    printf(" t_cmd->args matches bash argument parsing \n");
    printf("\n💡 This test verifies that the parsing pipeline correctly\n");
    printf("   generates the same argument list that bash would receive.\n\n");    
    
    index 			= 0;
    test_number 	= 0;
    passed 			= 0;
    failed 			= 0;
    syntax_errors 	= 0;
    failed_count 	= 0;
    failed_tests	= malloc(sizeof(int) * 1000); // Tamaño suficiente para todos los tests
    
    while (test_cases[index])
    {
        test_number++;
        printf("📋 TEST %d -> \t\t < %s >\n\n", test_number, test_cases[index]);  	     
        
        shell->input = ft_strdup(test_cases[index]);
        bool test_passed = true;        
        if (validate_syntax(shell) == SYNTAX_ERROR)			// FASE 0: Validación de sintaxis
        {
            if (is_expected_syntax_error(test_cases[index]))
            {
                printf("✅ SYNTAX ERROR DETECTED -> \t Correctly identified as invalid syntax\n");
                syntax_errors++;
                passed++; 	// Contar como éxito porque detectó el error correctamente
            }
            else
            {
                printf("❌ UNEXPECTED SYNTAX ERROR -> \t Should be valid syntax\n");
                failed++;
                failed_tests[failed_count++] = test_number; // Guardar número de test fallido
            }
            printf("──────────────────────────────────────\n\n");
            test_passed = false;
        }
        if (test_passed)			// FASE 1: Syntax analyzer
        {            
			create_commands_structure(shell);
			if (!shell->commands_list)
			{
				printf("❌ FAILED -> \t Syntax analyzer failed to create command structure\n");
				failed++;
				failed_tests[failed_count++] = test_number;
				test_passed = false;
			}
			else
			{
				if (validate_command_structure(shell) == SYNTAX_ERROR)
				{
					printf("❌ FAILED -> \t Command structure invalid\n");
					failed++;
					failed_tests[failed_count++] = test_number;
					test_passed = false;
				}
			}
        }         
        if (test_passed)		// FASE 3: Process commands  
        {
            process_commands(shell);            
            // Verificar que el procesamiento fue exitoso
            if (!shell->commands_list->words_list)
            {
                printf("❌ FAILED -> \t Command processing pipeline failed\n");
                failed++;
                failed_tests[failed_count++] = test_number;
                test_passed = false;
            }
            else
            {
                if (validate_command_semantics(shell) == SYNTAX_ERROR)
                {
                    printf("❌ FAILED -> \t Command semantics invalid\n");
                    failed++;
                    failed_tests[failed_count++] = test_number;
                    test_passed = false;
                }
            }
        }        
        if (test_passed)	// FASE 4: Build execution structure
        {
            build_execution_structure(shell->commands_list, shell);
            if (!shell->commands_list->args)
            {
                printf("❌ FAILED -> \t Build execution failed\n");
                failed++;
                failed_tests[failed_count++] = test_number; // Guardar número de test fallido
                test_passed = false;
            }
        }    
        if (test_passed)		// Comparar con bash solo si el test pasó todas las fases
        {           
            minishell_args = shell->commands_list->args;			// Obtener argumentos de minishell            
            bash_args = get_bash_args_real(test_cases[index]);		// Obtener argumentos esperados (simulando bash)            
            print_args_array(bash_args, "Expected (bash)");
            print_args_array(minishell_args, "Actual (minishell)");            
            if (compare_args_arrays(minishell_args, bash_args))		// Comparar arrays
            {
                printf("✅ PASSED -> \t Arguments match\n");
                passed++;
            }
            else
            {
                printf("❌ FAILED -> \t Arguments don't match\n");
                failed++;
                failed_tests[failed_count++] = test_number; 		// Guardar número de test fallido
            }            
            if (bash_args)
                free_matrix(bash_args);
        }        
        printf("──────────────────────────────────────\n\n");
        
        // Cleanup después de cada test
        free(shell->input);
        if (shell->commands_list)
            free_commands_list(&shell->commands_list);
        shell->input = NULL;
        shell->commands_list = NULL;
        shell->exit_status = 0; 				// Reset exit status        
        index++;
    }    
    cleanup_minishell(shell);
    
    printf("\n📊 PARSER ARGUMENT TEST SUMMARY\n");
    printf("================================\n");
    printf("✅ Passed: \t\t %d/%d\n", passed, test_number);
    printf("❌ Failed: \t\t %d/%d\n", failed, test_number);
    printf("Success rate: \t %.1f %%\n", (float) passed / (float) test_number * 100);    
    
    if (syntax_errors > 0)		// Mostrar desglose de resultados
    {
        printf("\n📝 Test breakdown:\n");
        printf("   • Valid syntax tests: \t %d/%d\n", passed - syntax_errors, test_number - syntax_errors);
        printf("   • Syntax error tests: \t %d (correctly detected)\n", syntax_errors);
    }    
    if (failed == 0)
        printf("🎉 All argument parsing tests passed!\n");
    else
    {
        printf("⚠️ %d tests need attention: [", failed);
        for (int i = 0; i < failed_count; i++)
        {
            printf("%d", failed_tests[i]);
            if (i < failed_count - 1)
                printf(", ");
        }
        printf("]\n");
    }
    free(failed_tests);
    exit(0);
}

static bool is_expected_syntax_error(char *input)
{
    int index;
	int patterns_index;
	char *input_trimmed;
    
    if (!input)
        return false;   
    index 			= 0;
	patterns_index 	= 0;
    while (input[index] && is_space(input[index]))		
        index++;    
    // Lista de patrones que deberían dar error de sintaxis
    char *error_patterns[] = {
        "| echo hello",           
        "|echo hello",            
        "echo hello |",           
        "echo hello ||",          
        "echo hello |||",         
        "|| echo hello",          
        "| |",                    
        "||",                     
        "| | echo hello | |",     
        "|$USER",                 
        "echo hello |		",     
        "	|	echo hello",       
        "echo hello \\n| echo world",
		"echo 'hello | world' |",
        "echo hola >>>>>>> file", 
        "echo hola <<<<<<< file", 
        "echo hola </<</>/>>",    
        "<< EOF",                 
        "echo hola > file </<</>/>>",
		"| cat << EOF",
		"cat << EOF | ",
		"echo \"hello | world\" | | echo test",
		"$USER|",
        " | | ",        
        NULL
    };
    while (error_patterns[patterns_index])
    {
        if (ft_strncmp(&input[index], error_patterns[patterns_index], ft_strlen(error_patterns[patterns_index])) == 0)
            return true;
        patterns_index++;
    }    
    // Verificaciones adicionales con patrones generales
    input_trimmed = &input[index];    
	if (ft_strnstr(input_trimmed, ">>>", ft_strlen(">>>"))
		|| ft_strnstr(input_trimmed, "<<<", ft_strlen("<<<"))) 		// Múltiples redirecciones consecutivas
        return true; 
	if (ft_strnstr(input_trimmed, "||", ft_strlen("||")))				// Double pipes
        return true;    
	if (ft_strnstr(input_trimmed, "</<</>/>>", ft_strlen("</<</>/>>")))	// Operadores mezclados
        return true;  
    return false;
}

// Función auxiliar para obtener argumentos que bash recibiría
static char **get_bash_args_real(char *input)
{
    int 	pipefd[2];
    pid_t 	pid;
    char 	buffer[4096];
    char 	**args;
    int 	bytes_read;
    char 	*bash_command;
    
    if (!input || !*input)
        return (NULL);
    args = NULL;    
    bash_command = malloc(strlen(input) + 50);		// Crear comando bash que ejecute e imprima argumentos
    sprintf(bash_command, "printf \"%%s\\n\" %s", input);    
    if (pipe(pipefd) == -1)
    {
        free(bash_command);
        return (NULL);
    }    
    pid = fork();
    if (pid == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execl("/bin/bash", "bash", "-c", bash_command, NULL);
        exit(1);
    }    
    close(pipefd[1]);
    wait(NULL);    
    bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
    close(pipefd[0]);
    free(bash_command);    
    if (bytes_read > 0)
    {
        buffer[bytes_read] = '\0';
        if (buffer[bytes_read - 1] == '\n')
            buffer[bytes_read - 1] = '\0';
        args = ft_split(buffer, '\n');
    }    
    return (args);
}

// Función para comparar arrays de argumentos
static bool compare_args_arrays(char **minishell_args, char **bash_args)
{
    int index;
    
    if (!minishell_args && !bash_args)
        return (true);  
    if (!minishell_args || !bash_args)
		return (false);
	index = 0;    
    while (minishell_args[index] && bash_args[index])
    {
        if (ft_strncmp(minishell_args[index], bash_args[index], ft_strlen(bash_args[index])) != 0)
            return (false);
        index++;
    }    
    return (minishell_args[index] == NULL && bash_args[index] == NULL); // Verificar que ambos arrays terminan al mismo tiempo
}

// Función para imprimir array de argumentos
static void print_args_array(char **args, const char *label)
{
    int index;
	
	index = 0;    
    printf("%s ->\t [", label);
    if (args)
    {
        while (args[index])
        {
            printf("\"%s\"", args[index]);
            if (args[index + 1])
                printf(", ");
            index++;
        }
    }
    printf("]\n\n");
}

