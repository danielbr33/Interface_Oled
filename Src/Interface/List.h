#include "List_element.h"
#ifndef LIST_H_
#define LIST_H_

class Back ;
class List{
private:

    List_element *current_element;
    List_element *last_element ;
    List_element *first_element ;

    void addFirstElement(List_element *element);
    void addNextElement(List_element *element);

    uint16_t size = false;

public :
    List();
    Parameter* getParameter();
    void addParameter(Parameter *parameter) ;
    void moveRight() ;
    void moveLeft() ;
    uint16_t getSize() ;
    void resetSubList() ;
    void print();
    uint8_t ifLastListElement() ;
};
#endif /* LIST_H_ */
