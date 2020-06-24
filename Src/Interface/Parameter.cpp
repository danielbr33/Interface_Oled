#include "Parameter.h"
#include "List.h"
//#define DEBUG
//#include <windows.h>
Parameter::Parameter(string h, int v, string u , uint8_t ch ){ //inSubList
	headline = h;
	value = v ;
	unit = u ;
	if_changeable_value = ch;
	edit_mode = false ;
}

void Parameter::createList(){
	if(has_sub_list == false){
		list = new List();
		has_sub_list = true;
    }
}
List* Parameter::getSubList(){
    if( has_sub_list)
        return list ;
}
void Parameter::addToSubList(Parameter *p){
	if(has_sub_list){
		list->addParameter(p);
	}
}

uint8_t Parameter::ifHasSubList(){
    return has_sub_list ;
}

uint8_t Parameter::ifInSubList(){
    return in_sub_list ;
}

void Parameter::sendToDisplay()
{
	system("cls");

	cout << headline << endl;
	/*if( edit_mode )
        glimmerValue(value) ;
    else*/
    if( has_sub_list == false )
        cout << value << " " << unit ;
	//cout << endl;
#ifdef DEBUG
	cout << endl << endl;
	if(has_sub_list){
		cout << "This parameter has sub list" << endl;
		list->print();
	} else {
		cout << "This parameter has no sub list" << endl;
	}

#endif


}
/*
void Parameter::glimmerValue(int gValue){
    while(1){
        cout << gValue ;
        Sleep(1000) ;
        for(int i=gValue ; i > 0 ; i /= 10 )
            cout << "\b \b" ;
        Sleep(500) ;
    }
}
*/
void Parameter::sendErrorNoChangeable(){

    system("cls");
	cout <<"No change" << endl ;
	cout <<"possible" ;
}

Parameter* Parameter::getParametr(){
   if(has_sub_list && in_sub_list)
      return list->getParameter();
   else
       return this ;
}

Interface_Element::Action Parameter::getButton(Interface_Element::Button button){

    if( button == Interface_Element::ENTER){
        if(edit_mode){
                edit_mode = false ;
        }
        else{
            if(if_changeable_value)
                edit_mode = true ;
            else if(has_sub_list){
                 in_sub_list = true ;
            }

            else
                return Interface_Element::ERROR_NO_CHANGEABLE;
        }
        return Interface_Element::DO_NOTHING ;
    }
    else if(edit_mode) {
		if(button == Interface_Element::RIGHT_BUTTON){
			value++;
			return Interface_Element::DO_NOTHING ;
		}
		else if(button == Interface_Element::LEFT_BUTTON){
            value--;
            return Interface_Element::DO_NOTHING ;
		}

    }
    else if( button == Interface_Element::LEFT_BUTTON){
        return Interface_Element::MOVE_LEFT ;
    }
    else if( button == Interface_Element::RIGHT_BUTTON){
        return Interface_Element::MOVE_RIGHT ;
    }
}


string Parameter::getHeadLine(){
	return this->headline;
}

void Parameter::setOutOfSubList(){
    in_sub_list = false ;
    list->resetSubList() ;
}

void Parameter::newMove(Interface_Element::Action action){

   if(action == Interface_Element::MOVE_RIGHT){
       if( list->ifLastListElement()){
            this->setOutOfSubList() ;
        }
        else
            list->moveRight() ;
   }
    else if( action == Interface_Element::MOVE_LEFT)
        list->moveLeft() ;
}

