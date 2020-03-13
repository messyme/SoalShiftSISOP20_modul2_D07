#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

//template daemon dari modul
static void daemonize()
{
  // Variabel untuk menyimpan PID
  pid_t pid, sid;        

  // Menyimpan PID dari Child Process
  pid = fork();     

  /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
  if (pid < 0)
    exit(EXIT_FAILURE);

  /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
  if (pid > 0) 
    exit(EXIT_SUCCESS);

    umask(0);

    sid = setsid();
    if (sid < 0) 
      exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}


//b. program mengeluarkan pesan eror jika argumen yang diberikan tidak sesuai
void printError ()
{
    printf ("ERROR, Invalid Argument\n");
}


//main function
int main (int argc, char *argv[])
{
    //c. program berjalan di daemon
    daemonize();

    time_t t = time(NULL);
    struct tm currentTime = *localtime(&t);
    int detik, menit, jam;
    //-1 for anytime
    detik = -1; 
    menit = -1;
    jam = -1;

    if (argc != 5) {
        printError ();
        exit(EXIT_FAILURE);
    }

    //a. 
    //mengganti argumen yang tadinya string menjadi int
    if (argv[1][0] != '*') 
        detik = atoi(argv[1]); 
    if (argv[2][0] != '*') 
        menit = atoi(argv[2]);
    if (argv[3][0] != '*') 
        jam = atoi(argv[3]);
    
    if (jam < -1 || jam > 23 || menit < -1 || menit > 59 || detik < -1 || detik > 59) {
        printError ();
        exit(EXIT_FAILURE);
    }

    while (1) { //crontab jd nerus jalan
        t = time(NULL); //biar update timenya sesuai current time
        struct tm currentTime = *localtime(&t);
        //bakal jalan kalo jamnya memenuhi syarat sesuai current time
        //* berarti argumen yg masuk itu true.
        if ((currentTime.tm_hour == jam || jam == -1) && (currentTime.tm_min == menit || menit == -1) && (currentTime.tm_sec == detik || detik == -1)) { //kalo * == true atau sudah memenuhi current time
            //tidak menggunakan system()
            //execl bikin program mati, jd pake fork == 0 yaitu bikin child, sehingga execl ini jd child
            if (fork()==0) execl("/bin/bash", "bash", argv[argc-1], NULL); 
        }
        sleep(1); //delay 1 detik  
    }
}