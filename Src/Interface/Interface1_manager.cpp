#include "Interface1_manager.h"
#define BUTTON_1 75 // left
#define BUTTON_2 77 // right
#define BUTTON_3 80 // down
#define SPECIAL_BUTTON 224

Interface1_manager::Interface1_manager(){

    Ssd_1306 = new Interface1 ;
    while(true){
        if( getch() == SPECIAL_BUTTON){
            print(Ssd_1306->getAction( readKey() ) );
        }
    }
}

Interface_Element::Button Interface1_manager::readKey(){
    if(kbhit()){
        switch(getch())
        {
            case BUTTON_1:
                return Interface_Element::LEFT_BUTTON;
            case BUTTON_2:
                return Interface_Element::RIGHT_BUTTON;
            case BUTTON_3:
                return Interface_Element::ENTER ;
            break;
        }
    }
}

void Interface1_manager::print(Interface_Element::Action info) {

    if( info == Interface_Element::PRINT){
        Ssd_1306->display() ;
    }
    else if( info ==Interface_Element::ERROR_NO_CHANGEABLE){
        Ssd_1306->displayError() ;
    }
}
