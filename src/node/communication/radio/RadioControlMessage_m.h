//
// Generated file, do not edit! Created by nedtool 5.6 from node/communication/radio/RadioControlMessage.msg.
//

#ifndef __RADIOCONTROLMESSAGE_M_H
#define __RADIOCONTROLMESSAGE_M_H

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
 * Enum generated from <tt>node/communication/radio/RadioControlMessage.msg:14</tt> by nedtool.
 * <pre>
 * // These messages will be initiated by the radio and sent to upper layer
 * enum RadioControlMessage_type
 * {
 *     CARRIER_SENSE_INTERRUPT = 1;
 *     RADIO_BUFFER_FULL = 2;
 * }
 * </pre>
 */
enum RadioControlMessage_type {
    CARRIER_SENSE_INTERRUPT = 1,
    RADIO_BUFFER_FULL = 2
};

/**
 * Class generated from <tt>node/communication/radio/RadioControlMessage.msg:19</tt> by nedtool.
 * <pre>
 * message RadioControlMessage
 * {
 *     int radioControlMessageKind \@enum(RadioControlMessage_type);
 * }
 * </pre>
 */
class RadioControlMessage : public ::omnetpp::cMessage
{
  protected:
    int radioControlMessageKind;

  private:
    void copy(const RadioControlMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const RadioControlMessage&);

  public:
    RadioControlMessage(const char *name=nullptr, short kind=0);
    RadioControlMessage(const RadioControlMessage& other);
    virtual ~RadioControlMessage();
    RadioControlMessage& operator=(const RadioControlMessage& other);
    virtual RadioControlMessage *dup() const override {return new RadioControlMessage(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getRadioControlMessageKind() const;
    virtual void setRadioControlMessageKind(int radioControlMessageKind);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const RadioControlMessage& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, RadioControlMessage& obj) {obj.parsimUnpack(b);}

/**
 * Enum generated from <tt>node/communication/radio/RadioControlMessage.msg:23</tt> by nedtool.
 * <pre>
 * enum BasicState_type
 * {
 *     RX = 0;
 *     TX = 1;
 *     SLEEP = 2;
 * }
 * </pre>
 */
enum BasicState_type {
    RX = 0,
    TX = 1,
    SLEEP = 2
};

/**
 * Enum generated from <tt>node/communication/radio/RadioControlMessage.msg:30</tt> by nedtool.
 * <pre>
 * // These commands will be received by the radio from upper layers
 * enum RadioControlCommand_type
 * {
 *     SET_STATE = 0;
 *     SET_MODE = 1;
 *     SET_TX_OUTPUT = 2;
 *     SET_SLEEP_LEVEL = 3;
 *     SET_CARRIER_FREQ = 4;
 *     SET_CCA_THRESHOLD = 5;
 *     SET_CS_INTERRUPT_ON = 6;
 *     SET_CS_INTERRUPT_OFF = 7;
 *     SET_ENCODING = 8;
 * }
 * </pre>
 */
enum RadioControlCommand_type {
    SET_STATE = 0,
    SET_MODE = 1,
    SET_TX_OUTPUT = 2,
    SET_SLEEP_LEVEL = 3,
    SET_CARRIER_FREQ = 4,
    SET_CCA_THRESHOLD = 5,
    SET_CS_INTERRUPT_ON = 6,
    SET_CS_INTERRUPT_OFF = 7,
    SET_ENCODING = 8
};

/**
 * Class generated from <tt>node/communication/radio/RadioControlMessage.msg:42</tt> by nedtool.
 * <pre>
 * message RadioControlCommand
 * {
 *     int radioControlCommandKind \@enum(RadioControlCommand_type);
 *     int state \@enum(BasicState_type) = RX;	//to be used with SET_STATE
 *     double parameter = 0.0;	//to be used with SET_TX_OUTPUT, SET_CARRIER_FREQ, SET_CCA_THRESHOLD
 * }
 * </pre>
 */
class RadioControlCommand : public ::omnetpp::cMessage
{
  protected:
    int radioControlCommandKind;
    int state;
    double parameter;

  private:
    void copy(const RadioControlCommand& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const RadioControlCommand&);

  public:
    RadioControlCommand(const char *name=nullptr, short kind=0);
    RadioControlCommand(const RadioControlCommand& other);
    virtual ~RadioControlCommand();
    RadioControlCommand& operator=(const RadioControlCommand& other);
    virtual RadioControlCommand *dup() const override {return new RadioControlCommand(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getRadioControlCommandKind() const;
    virtual void setRadioControlCommandKind(int radioControlCommandKind);
    virtual int getState() const;
    virtual void setState(int state);
    virtual double getParameter() const;
    virtual void setParameter(double parameter);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const RadioControlCommand& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, RadioControlCommand& obj) {obj.parsimUnpack(b);}


#endif // ifndef __RADIOCONTROLMESSAGE_M_H

