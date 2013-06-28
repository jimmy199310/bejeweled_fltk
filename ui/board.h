#ifndef BOARD_H_
#define BOARD_H_

class IMainBoard {
public:
/*    IMainBoard(){};
    ~IMainBoard(){};
 */  
    virtual void init() = 0;
    virtual void setTitle() = 0;
};

#endif