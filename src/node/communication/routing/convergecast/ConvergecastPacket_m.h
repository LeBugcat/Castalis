//
// Generated file, do not edit! Created by nedtool 5.6 from node/communication/routing/convergecast/ConvergecastPacket.msg.
//

#ifndef __CONVERGECASTPACKET_M_H
#define __CONVERGECASTPACKET_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0506
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
#include "node/communication/routing/RoutingPacket_m.h"
#include "helpStructures/GeoMathHelper.h"
typedef std::vector<int> IntVector;
// }}

/**
 * Enum generated from <tt>node/communication/routing/convergecast/ConvergecastPacket.msg:27</tt> by nedtool.
 * <pre>
 * enum ConvergecastPacket_Type
 * {
 *     CC_ROUTING_CONTROL_PACKET = 1;
 *     CC_ROUTING_DATA_PACKET = 2;
 *     CC_ROUTING_JOIN_PACKET = 3;
 *     CC_ROUTING_ADV_PACKET = 4;
 * }
 * </pre>
 */
enum ConvergecastPacket_Type {
    CC_ROUTING_CONTROL_PACKET = 1,
    CC_ROUTING_DATA_PACKET = 2,
    CC_ROUTING_JOIN_PACKET = 3,
    CC_ROUTING_ADV_PACKET = 4
};

/**
 * Class generated from <tt>node/communication/routing/convergecast/ConvergecastPacket.msg:35</tt> by nedtool.
 * <pre>
 * packet ConvergecastPacket extends RoutingPacket
 * {
 *     int ConvergecastPacketKind \@enum(ConvergecastPacket_Type);
 *     int sourceCH;
 *     int sourceRound;
 *     double remainingEnergy;
 *     IntVector isCH;
 *     IntVector nextCH;
 *     IntVector trace;
 * }
 * </pre>
 */
class ConvergecastPacket : public ::RoutingPacket
{
  protected:
    int ConvergecastPacketKind;
    int sourceCH;
    int sourceRound;
    double remainingEnergy;
    IntVector isCH;
    IntVector nextCH;
    IntVector trace;

  private:
    void copy(const ConvergecastPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const ConvergecastPacket&);

  public:
    ConvergecastPacket(const char *name=nullptr, short kind=0);
    ConvergecastPacket(const ConvergecastPacket& other);
    virtual ~ConvergecastPacket();
    ConvergecastPacket& operator=(const ConvergecastPacket& other);
    virtual ConvergecastPacket *dup() const override {return new ConvergecastPacket(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getConvergecastPacketKind() const;
    virtual void setConvergecastPacketKind(int ConvergecastPacketKind);
    virtual int getSourceCH() const;
    virtual void setSourceCH(int sourceCH);
    virtual int getSourceRound() const;
    virtual void setSourceRound(int sourceRound);
    virtual double getRemainingEnergy() const;
    virtual void setRemainingEnergy(double remainingEnergy);
    virtual IntVector& getIsCH();
    virtual const IntVector& getIsCH() const {return const_cast<ConvergecastPacket*>(this)->getIsCH();}
    virtual void setIsCH(const IntVector& isCH);
    virtual IntVector& getNextCH();
    virtual const IntVector& getNextCH() const {return const_cast<ConvergecastPacket*>(this)->getNextCH();}
    virtual void setNextCH(const IntVector& nextCH);
    virtual IntVector& getTrace();
    virtual const IntVector& getTrace() const {return const_cast<ConvergecastPacket*>(this)->getTrace();}
    virtual void setTrace(const IntVector& trace);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const ConvergecastPacket& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, ConvergecastPacket& obj) {obj.parsimUnpack(b);}


#endif // ifndef __CONVERGECASTPACKET_M_H

