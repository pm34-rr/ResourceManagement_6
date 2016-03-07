#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/wait.h>

char msg[100];		//! Переменная для передачи сообщений в главный процесс
int messages[2];	//! Дескрипторы для канала сообщений

/*!
 * \brief	Вывод сообщения от процесса и сообщение о его закрытии
 * \param i Номер процесса
 */
void closeProcess( int i )
{
	read( messages[0], msg, sizeof( msg ) );
	printf( msg );
	printf( "Process %d is closed\n", i );
}

int main()
{
	int p1_fd[2]; //! Дескрипторы для канала, в котором хранится результат cat a.txt ...

	//! Создание каналов
	pipe( messages );
	pipe( p1_fd );

	printf( "Creating process 1\n" );
	if ( fork() == 0 ) {
		//! сat part

		close( 1 );			//! Закрыли стандартный вывод
		dup( p1_fd[1] );	//! Назначили наш дескриптор для стандартного вывода

		sprintf( msg, "|---Exec cat a.txt b.txt c.txt\n" ); //! Запись в переменную msg сообщения о выполняемой команде
		write( messages[1], msg, sizeof( msg ) );			//! Запись в канал перемнной msg
		execl( "/bin/cat", "cat", "a.txt", "b.txt", "c.txt", NULL ); //! Запуск команды cat с необходимыми параметрами
		exit( 0 );
	}

	sleep( 1 );
	closeProcess( 1 );

	//! Создание канала для хранения результата tr -d "[a-i]"
	int p2_fd[2];
	pipe( p2_fd );

	printf( "Creating process 2\n" );
	if ( fork() == 0 ) {
		//! tr -d part

		close( p1_fd[1] );	//! Закрыли вывод канала p1_fd
		close( 0 );			//! Закрыли стандартный ввод
		dup( p1_fd[0] );	//! Назначили наш дескриптор для стандартного ввода

		close( 1 );			//! Закрыли стандартного вывод
		dup( p2_fd[1] );	//! Назначили наш дескриптор для стандартного вывода

		sprintf( msg, "|---Exec tr -d \"[a-i]\"\n" );	//! Запись в переменную msg сообщения о выполняемой команде
		write( messages[1], msg, sizeof( msg ) );		//! Запись в канал перемнной msg
		execl( "/usr/bin/tr", "tr", "-d", "\"[a-i]\"", NULL ); //! Запуск команды tr с необходимыми параметрами
		exit( 0 );
	}

	sleep( 1 );
	closeProcess( 2 );

	printf( "Creating process 3\n" );
	if ( fork() == 0 ) {
		//! wc -w part

		close( p2_fd[1] );	//! Закрыли вывод канала p2_fd
		close( p1_fd[1] );	//! Закрыли вывод канала p1_fd
		close( 0 );			//! Закрыли стандартного ввода
		dup( p2_fd[0] );	//! Назначили наш дескриптор лоя стандартного ввода

		sprintf( msg, "|---Exec wc -w\n" );			//! Запись в переменную msg сообщения о выполняемой команде
		write( messages[1], msg, sizeof( msg ) );	//! Запись в канал перемнной msg
		execl( "/usr/bin/wc", "wc", "-w", NULL );	//! Запуск команды wc с необходимыми параметрами
		exit( 0 );
	}
	int errorStatus;
	wait( &errorStatus );

	closeProcess( 3 );
	printf( "===== Result =====\n" );

	return 0;
}
