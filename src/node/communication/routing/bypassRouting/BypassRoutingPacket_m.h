//
// Generated file, do not edit! Created by nedtool 5.6 from node/communication/routing/bypassRouting/BypassRoutingPacket.msg.
//

#ifndef __BYPASSROUTINGPACKET_M_H
#define __BYPASSROUTINGPACKET_M_H

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
// }}

/**
 * Class generated from <tt>node/communication/routing/bypassRouting/BypassRoutingPacket.msg:19</tt> by nedtool.
 * <pre>
 * packet BypassRoutingPacket extends RoutingPacket
 * {
 * }
 * </pre>
 */
class BypassRoutingPacket : public ::RoutingPacket
{
  protected:

  private:
    void copy(const BypassRoutingPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const BypassRoutingPacket&);

  public:
    BypassRoutingPacket(const char *name=nullptr, short kind=0);
    BypassRoutingPacket(const BypassRoutingPacket& other);
    virtual ~BypassRoutingPacket();
    BypassRoutingPacket& operator=(const BypassRoutingPacket& other);
    virtual BypassRoutingPacket *dup() const override {return new BypassRoutingPacket(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const BypassRoutingPacket& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, BypassRoutingPacket& obj) {obj.parsimUnpack(b);}


#endif // ifndef __BYPASSROUTINGPACKET_M_H

