#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>


int main () 
{
    pid_t child_id1;
    child_id1 = fork ();
    int status;

    pid_t child_id2;
    child_id2 = fork ();

    pid_t child_unzip;
    
    pid_t child_type;

    int j = 0;
    int k = 0;

    char filename[256][256];
    char filenamed[256][256];


    if (child_id1 < 0) {
        exit (EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }
    printf ("suda masuk sini\n");

    // A.
    if (child_id1 == 0) {
        if (child_id2 < 0) {
            exit (EXIT_FAILURE);
        }
        if (child_id2 == 0) {
            // this is child
            char *argv[] = {"mkdir", "-p", "modul2/indomie", NULL};
            execv("/bin/mkdir", argv);
        }
        else {
            // this is parent
            while ((wait(&status)) > 0);
            char *argv[] = {"mkdir", "-p", "modul2/sedaap", NULL};
            execv("/bin/mkdir", argv);
        }
        //printf ("suda masuk sini\n");

    }
   
    else {
        printf ("tes\n");
        while ((wait(&status)) > 0);
            child_unzip = fork ();

        if (child_unzip == 0) {
            // B. unzip
            char *argv[] = {"unzip", "/home/maisie/modul2/jpg.zip", NULL};
            execv("/usr/bin/unzip", argv);
        }
        else {
            printf ("cek\n");
            //C. unzip to folder jpg
            while ((wait(&status)) > 0);
                child_type = fork ();
            printf ("kok gabisa sih\n");
            if (child_type == 0) {
                pid_t kelompok, move;
                int nstatus;
                DIR *d;
                struct dirent *dir;
                printf ("sudaa masuk sini\n");
                if ((d = opendir("/home/maisie/modul2/jpg")) != NULL) {
                    while ((dir = readdir(d)) != NULL) {
                        if (dir->d_type == DT_REG) {
                            strcpy (filename[j], dir->d_name); //file
                            j++;
                        }
                        else {
                            strcpy (filenamed[k], dir->d_name); //dir
                            k++;
                        }
                    }
                    printf ("suuda masuk sini\n");
                    closedir (d);
                    printf ("suda mmasuk sinii\n");
                }
                printf ("ssuda masuk sini\n");
                //C. sedaap
                for (int i=0; i<j; i++) {
                    kelompok = fork();
                    char name[25];
                    sprintf (name, "modul2/jpg/%s", filename[i]);
                    if (kelompok == 0) {
                        char *argv[] = {"mv", name, "/home/maisie/modul2/sedaap/", NULL};
                        execv("/bin/mv", argv);
                        printf ("sudaH masuk sini\n");
                    }
                }
                //c. indomie
                for (int i=2; i<k; i++) {
                    kelompok = fork();
                    char name[25];
                    sprintf (name, "modul2/jpg/%s", filenamed[i]);
                    if (kelompok == 0) {
                        char *argv[] = {"mv", name, "/home/maisie/modul2/indomie/", NULL};
                        execv("/bin/mv", argv);
                        printf ("sudaHH masuk sini\n");
                    }
                    else {
                        return 0;
					}
                }
            }
        }
    }
    return 0;
}