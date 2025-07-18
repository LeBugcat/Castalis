//
// Generated file, do not edit! Created by nedtool 5.6 from physicalProcess/PhysicalProcessMessage.msg.
//

#ifndef __PHYSICALPROCESSMESSAGE_M_H
#define __PHYSICALPROCESSMESSAGE_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0506
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>physicalProcess/PhysicalProcessMessage.msg:13</tt> by nedtool.
 * <pre>
 * message PhysicalProcessMessage
 * {
 *     double value;
 *     double xCoor;
 *     double yCoor;
 *     int srcID;		//the ID of the node module that sends the Sample request
 *     int sensorIndex;
 * }
 * </pre>
 */
class PhysicalProcessMessage : public ::omnetpp::cMessage
{
  protected:
    double value;
    double xCoor;
    double yCoor;
    int srcID;
    int sensorIndex;

  private:
    void copy(const PhysicalProcessMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const PhysicalProcessMessage&);

  public:
    PhysicalProcessMessage(const char *name=nullptr, short kind=0);
    PhysicalProcessMessage(const PhysicalProcessMessage& other);
    virtual ~PhysicalProcessMessage();
    PhysicalProcessMessage& operator=(const PhysicalProcessMessage& other);
    virtual PhysicalProcessMessage *dup() const override {return new PhysicalProcessMessage(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual double getValue() const;
    virtual void setValue(double value);
    virtual double getXCoor() const;
    virtual void setXCoor(double xCoor);
    virtual double getYCoor() const;
    virtual void setYCoor(double yCoor);
    virtual int getSrcID() const;
    virtual void setSrcID(int srcID);
    virtual int getSensorIndex() const;
    virtual void setSensorIndex(int sensorIndex);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const PhysicalProcessMessage& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, PhysicalProcessMessage& obj) {obj.parsimUnpack(b);}


#endif // ifndef __PHYSICALPROCESSMESSAGE_M_H

