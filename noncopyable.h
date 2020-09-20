#ifndef E_S_NONCOPYABLE_H
#define E_S_NONCOPYABLE_H

/**
 * the class derived from noncopyable class do not have copy constructor 
 * and assignment operators but can have constructor and destructor. 
 */

class noncopyable
{
public:
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
protected:
    noncopyable() = default;
    ~noncopyable() = default;
};




#endif