#include "zagolovok.h"
#include <iostream>
#include <math.h>
#include <string.h>
using namespace std;
int main(){int way,k;
	cout<<"\n            WELCOME TO AIR COMPETITION!\n";
	cout<<"             ___________\n             \\_________/\n	      |WINNER!|\n	       \\_____/\n	         | |\n                 | |\n                /___\\\n";
    cout<<"\n            ENTER THE LAST WINNER!\n";
 Air_competition* currentWinner = new Air_competition(); //
 cin >> *currentWinner; // 
 cout << *currentWinner; // 
 cout<<"\n  ENTER LENGTH OF WAY:>    ";
 cin>>way;
 Airplane* plane =new Airplane(); 
 cin >> *plane; 
 cout << *plane;
 Balloon* balloon=new Balloon();
 cin>>*balloon;
 cout<<*balloon;
 Bird* bird=new Bird();
 cin>>*bird;
 cout<<*bird;
 if((*plane<way)==false)cout<<"\nPLANE WON'T REACH FINISH! NOT ENOUGH FUEL!\n";
 if((*balloon<way)==false) cout<<"\nBALLOON WON'T REACH FINISH! NOT ENOUGH GAS!\n";
 if((--(*bird))==false)cout<<"\nBIRD WON'T REACH FINISH! NOT ENOUGH POWER!\n";
 ++(*bird);//ptica ustala;
 
if(*plane<way){currentWinner=dynamic_cast<Air_competition*>(plane);
     if((*balloon<way)&&(*balloon>*currentWinner)){currentWinner=dynamic_cast<Air_competition*>(balloon);
	 k=++(*balloon);
	 change_speed( *currentWinner,k);
	    if((--(*bird))&&(*bird>*currentWinner)){ cout<<"        WINNER IS: "<<*bird;
         cout<<"\n    OUR CONGRATULATIONS!\n";}
	       else{cout<<"WINNER IS: "<<*balloon;
           cout<<"\n    OUR CONGRATULATIONS!\n";}
	}
     else if((--(*bird))&&(*bird>*currentWinner)){ cout<<"      WINNER IS: "<<*bird;
     cout<<"\n   OUR CONGRATULATIONS!\n";}
	 else{cout<<"Pobeditel: "<<*plane;
     cout<<"\nO    OUR CONGRATULATIONS!\n";}
}
    else if(*balloon<way) {
    currentWinner=currentWinner=dynamic_cast<Air_competition*>(balloon);
	k=++(*balloon);
	change_speed( *currentWinner,k);
	

	if((--(*bird))&&(*bird>*currentWinner)){ cout<<"      WINNER IS: "<<*bird;
         cout<<"\n      OUR CONGRATULATIONS!\n";}
	       else{cout<<"    WINNER IS: "<<*balloon;
           cout<<"\nOUR CONGRATULATIONS!\n";}


	}
           else if(--(*bird)){
		   cout<<"WINNER IS: "<<*bird;
		   cout<<"\nOUR CONGRATULATIONS!\n";
		   }
               else{
			   cout<<"POBEDITELEY NET!\n";
			   }
cout<<"             ___________\n             \\_________/\n	      |WINNER!|\n	       \\_____/\n	         | |\n                 | |\n                /___\\\n";



return 0;
}
