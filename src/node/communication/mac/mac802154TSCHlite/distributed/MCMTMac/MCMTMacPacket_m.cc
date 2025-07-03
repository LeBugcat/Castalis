//
// Generated file, do not edit! Created by nedtool 5.6 from node/communication/mac/mac802154TSCHlite/distributed/MCMTMac/MCMTMacPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "MCMTMacPacket_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("MCMTMACPacket_type");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("MCMTMACPacket_type"));
    e->insert(HELLO, "HELLO");
    e->insert(ASSIGN, "ASSIGN");
    e->insert(INFO, "INFO");
    e->insert(BD_CLAIM, "BD_CLAIM");
    e->insert(BATCH_CLAIM, "BATCH_CLAIM");
    e->insert(DONE, "DONE");
)

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("MCMTMACGeoColor_type");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("MCMTMACGeoColor_type"));
    e->insert(GREEN, "GREEN");
    e->insert(RED, "RED");
    e->insert(PURPLE, "PURPLE");
    e->insert(YELLOW, "YELLOW");
)

Register_Class(MCMTMACPacket)

MCMTMACPacket::MCMTMACPacket(const char *name, short kind) : ::MAC802154LitePacket(name,kind)
{
    this->MCMTMACPacketType = 0;
    this->hasEncapsulatedPacket = false;
    this->isBlankChainAssign = false;
    this->encapsulatedNodeId = -1;
    this->encapsulatedParentId = -1;
    this->encapsulatedCHId = -1;
    this->encapsulatedTransmissionCount = -1;
    this->encapsulatedGeocolor = -1;
    this->parentId = 0;
    this->CHId = 0;
    this->sourceGeoColor = 0;
    this->transmissionCount = 0;
    batchClaimTimeslots_arraysize = 0;
    this->batchClaimTimeslots = 0;
    batchClaimChannelIds_arraysize = 0;
    this->batchClaimChannelIds = 0;
    this->singleClaimTimeslot = 0;
    this->singleChannelId = 0;
}

MCMTMACPacket::MCMTMACPacket(const MCMTMACPacket& other) : ::MAC802154LitePacket(other)
{
    batchClaimTimeslots_arraysize = 0;
    this->batchClaimTimeslots = 0;
    batchClaimChannelIds_arraysize = 0;
    this->batchClaimChannelIds = 0;
    copy(other);
}

MCMTMACPacket::~MCMTMACPacket()
{
    delete [] this->batchClaimTimeslots;
    delete [] this->batchClaimChannelIds;
}

MCMTMACPacket& MCMTMACPacket::operator=(const MCMTMACPacket& other)
{
    if (this==&other) return *this;
    ::MAC802154LitePacket::operator=(other);
    copy(other);
    return *this;
}

void MCMTMACPacket::copy(const MCMTMACPacket& other)
{
    this->MCMTMACPacketType = other.MCMTMACPacketType;
    this->hasEncapsulatedPacket = other.hasEncapsulatedPacket;
    this->isBlankChainAssign = other.isBlankChainAssign;
    this->encapsulatedNodeId = other.encapsulatedNodeId;
    this->encapsulatedParentId = other.encapsulatedParentId;
    this->encapsulatedCHId = other.encapsulatedCHId;
    this->encapsulatedTransmissionCount = other.encapsulatedTransmissionCount;
    this->encapsulatedGeocolor = other.encapsulatedGeocolor;
    this->parentId = other.parentId;
    this->CHId = other.CHId;
    this->sourceGeoColor = other.sourceGeoColor;
    this->transmissionCount = other.transmissionCount;
    delete [] this->batchClaimTimeslots;
    this->batchClaimTimeslots = (other.batchClaimTimeslots_arraysize==0) ? nullptr : new int[other.batchClaimTimeslots_arraysize];
    batchClaimTimeslots_arraysize = other.batchClaimTimeslots_arraysize;
    for (unsigned int i=0; i<batchClaimTimeslots_arraysize; i++)
        this->batchClaimTimeslots[i] = other.batchClaimTimeslots[i];
    delete [] this->batchClaimChannelIds;
    this->batchClaimChannelIds = (other.batchClaimChannelIds_arraysize==0) ? nullptr : new int[other.batchClaimChannelIds_arraysize];
    batchClaimChannelIds_arraysize = other.batchClaimChannelIds_arraysize;
    for (unsigned int i=0; i<batchClaimChannelIds_arraysize; i++)
        this->batchClaimChannelIds[i] = other.batchClaimChannelIds[i];
    this->singleClaimTimeslot = other.singleClaimTimeslot;
    this->singleChannelId = other.singleChannelId;
}

void MCMTMACPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::MAC802154LitePacket::parsimPack(b);
    doParsimPacking(b,this->MCMTMACPacketType);
    doParsimPacking(b,this->hasEncapsulatedPacket);
    doParsimPacking(b,this->isBlankChainAssign);
    doParsimPacking(b,this->encapsulatedNodeId);
    doParsimPacking(b,this->encapsulatedParentId);
    doParsimPacking(b,this->encapsulatedCHId);
    doParsimPacking(b,this->encapsulatedTransmissionCount);
    doParsimPacking(b,this->encapsulatedGeocolor);
    doParsimPacking(b,this->parentId);
    doParsimPacking(b,this->CHId);
    doParsimPacking(b,this->sourceGeoColor);
    doParsimPacking(b,this->transmissionCount);
    b->pack(batchClaimTimeslots_arraysize);
    doParsimArrayPacking(b,this->batchClaimTimeslots,batchClaimTimeslots_arraysize);
    b->pack(batchClaimChannelIds_arraysize);
    doParsimArrayPacking(b,this->batchClaimChannelIds,batchClaimChannelIds_arraysize);
    doParsimPacking(b,this->singleClaimTimeslot);
    doParsimPacking(b,this->singleChannelId);
}

void MCMTMACPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::MAC802154LitePacket::parsimUnpack(b);
    doParsimUnpacking(b,this->MCMTMACPacketType);
    doParsimUnpacking(b,this->hasEncapsulatedPacket);
    doParsimUnpacking(b,this->isBlankChainAssign);
    doParsimUnpacking(b,this->encapsulatedNodeId);
    doParsimUnpacking(b,this->encapsulatedParentId);
    doParsimUnpacking(b,this->encapsulatedCHId);
    doParsimUnpacking(b,this->encapsulatedTransmissionCount);
    doParsimUnpacking(b,this->encapsulatedGeocolor);
    doParsimUnpacking(b,this->parentId);
    doParsimUnpacking(b,this->CHId);
    doParsimUnpacking(b,this->sourceGeoColor);
    doParsimUnpacking(b,this->transmissionCount);
    delete [] this->batchClaimTimeslots;
    b->unpack(batchClaimTimeslots_arraysize);
    if (batchClaimTimeslots_arraysize==0) {
        this->batchClaimTimeslots = 0;
    } else {
        this->batchClaimTimeslots = new int[batchClaimTimeslots_arraysize];
        doParsimArrayUnpacking(b,this->batchClaimTimeslots,batchClaimTimeslots_arraysize);
    }
    delete [] this->batchClaimChannelIds;
    b->unpack(batchClaimChannelIds_arraysize);
    if (batchClaimChannelIds_arraysize==0) {
        this->batchClaimChannelIds = 0;
    } else {
        this->batchClaimChannelIds = new int[batchClaimChannelIds_arraysize];
        doParsimArrayUnpacking(b,this->batchClaimChannelIds,batchClaimChannelIds_arraysize);
    }
    doParsimUnpacking(b,this->singleClaimTimeslot);
    doParsimUnpacking(b,this->singleChannelId);
}

int MCMTMACPacket::getMCMTMACPacketType() const
{
    return this->MCMTMACPacketType;
}

void MCMTMACPacket::setMCMTMACPacketType(int MCMTMACPacketType)
{
    this->MCMTMACPacketType = MCMTMACPacketType;
}

bool MCMTMACPacket::getHasEncapsulatedPacket() const
{
    return this->hasEncapsulatedPacket;
}

void MCMTMACPacket::setHasEncapsulatedPacket(bool hasEncapsulatedPacket)
{
    this->hasEncapsulatedPacket = hasEncapsulatedPacket;
}

bool MCMTMACPacket::getIsBlankChainAssign() const
{
    return this->isBlankChainAssign;
}

void MCMTMACPacket::setIsBlankChainAssign(bool isBlankChainAssign)
{
    this->isBlankChainAssign = isBlankChainAssign;
}

int MCMTMACPacket::getEncapsulatedNodeId() const
{
    return this->encapsulatedNodeId;
}

void MCMTMACPacket::setEncapsulatedNodeId(int encapsulatedNodeId)
{
    this->encapsulatedNodeId = encapsulatedNodeId;
}

int MCMTMACPacket::getEncapsulatedParentId() const
{
    return this->encapsulatedParentId;
}

void MCMTMACPacket::setEncapsulatedParentId(int encapsulatedParentId)
{
    this->encapsulatedParentId = encapsulatedParentId;
}

int MCMTMACPacket::getEncapsulatedCHId() const
{
    return this->encapsulatedCHId;
}

void MCMTMACPacket::setEncapsulatedCHId(int encapsulatedCHId)
{
    this->encapsulatedCHId = encapsulatedCHId;
}

int MCMTMACPacket::getEncapsulatedTransmissionCount() const
{
    return this->encapsulatedTransmissionCount;
}

void MCMTMACPacket::setEncapsulatedTransmissionCount(int encapsulatedTransmissionCount)
{
    this->encapsulatedTransmissionCount = encapsulatedTransmissionCount;
}

int MCMTMACPacket::getEncapsulatedGeocolor() const
{
    return this->encapsulatedGeocolor;
}

void MCMTMACPacket::setEncapsulatedGeocolor(int encapsulatedGeocolor)
{
    this->encapsulatedGeocolor = encapsulatedGeocolor;
}

int MCMTMACPacket::getParentId() const
{
    return this->parentId;
}

void MCMTMACPacket::setParentId(int parentId)
{
    this->parentId = parentId;
}

int MCMTMACPacket::getCHId() const
{
    return this->CHId;
}

void MCMTMACPacket::setCHId(int CHId)
{
    this->CHId = CHId;
}

int MCMTMACPacket::getSourceGeoColor() const
{
    return this->sourceGeoColor;
}

void MCMTMACPacket::setSourceGeoColor(int sourceGeoColor)
{
    this->sourceGeoColor = sourceGeoColor;
}

int MCMTMACPacket::getTransmissionCount() const
{
    return this->transmissionCount;
}

void MCMTMACPacket::setTransmissionCount(int transmissionCount)
{
    this->transmissionCount = transmissionCount;
}

void MCMTMACPacket::setBatchClaimTimeslotsArraySize(unsigned int size)
{
    int *batchClaimTimeslots2 = (size==0) ? nullptr : new int[size];
    unsigned int sz = batchClaimTimeslots_arraysize < size ? batchClaimTimeslots_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        batchClaimTimeslots2[i] = this->batchClaimTimeslots[i];
    for (unsigned int i=sz; i<size; i++)
        batchClaimTimeslots2[i] = 0;
    batchClaimTimeslots_arraysize = size;
    delete [] this->batchClaimTimeslots;
    this->batchClaimTimeslots = batchClaimTimeslots2;
}

unsigned int MCMTMACPacket::getBatchClaimTimeslotsArraySize() const
{
    return batchClaimTimeslots_arraysize;
}

int MCMTMACPacket::getBatchClaimTimeslots(unsigned int k) const
{
    if (k>=batchClaimTimeslots_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", batchClaimTimeslots_arraysize, k);
    return this->batchClaimTimeslots[k];
}

void MCMTMACPacket::setBatchClaimTimeslots(unsigned int k, int batchClaimTimeslots)
{
    if (k>=batchClaimTimeslots_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", batchClaimTimeslots_arraysize, k);
    this->batchClaimTimeslots[k] = batchClaimTimeslots;
}

void MCMTMACPacket::setBatchClaimChannelIdsArraySize(unsigned int size)
{
    int *batchClaimChannelIds2 = (size==0) ? nullptr : new int[size];
    unsigned int sz = batchClaimChannelIds_arraysize < size ? batchClaimChannelIds_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        batchClaimChannelIds2[i] = this->batchClaimChannelIds[i];
    for (unsigned int i=sz; i<size; i++)
        batchClaimChannelIds2[i] = 0;
    batchClaimChannelIds_arraysize = size;
    delete [] this->batchClaimChannelIds;
    this->batchClaimChannelIds = batchClaimChannelIds2;
}

unsigned int MCMTMACPacket::getBatchClaimChannelIdsArraySize() const
{
    return batchClaimChannelIds_arraysize;
}

int MCMTMACPacket::getBatchClaimChannelIds(unsigned int k) const
{
    if (k>=batchClaimChannelIds_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", batchClaimChannelIds_arraysize, k);
    return this->batchClaimChannelIds[k];
}

void MCMTMACPacket::setBatchClaimChannelIds(unsigned int k, int batchClaimChannelIds)
{
    if (k>=batchClaimChannelIds_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", batchClaimChannelIds_arraysize, k);
    this->batchClaimChannelIds[k] = batchClaimChannelIds;
}

int MCMTMACPacket::getSingleClaimTimeslot() const
{
    return this->singleClaimTimeslot;
}

void MCMTMACPacket::setSingleClaimTimeslot(int singleClaimTimeslot)
{
    this->singleClaimTimeslot = singleClaimTimeslot;
}

int MCMTMACPacket::getSingleChannelId() const
{
    return this->singleChannelId;
}

void MCMTMACPacket::setSingleChannelId(int singleChannelId)
{
    this->singleChannelId = singleChannelId;
}

class MCMTMACPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    MCMTMACPacketDescriptor();
    virtual ~MCMTMACPacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(MCMTMACPacketDescriptor)

MCMTMACPacketDescriptor::MCMTMACPacketDescriptor() : omnetpp::cClassDescriptor("MCMTMACPacket", "MAC802154LitePacket")
{
    propertynames = nullptr;
}

MCMTMACPacketDescriptor::~MCMTMACPacketDescriptor()
{
    delete[] propertynames;
}

bool MCMTMACPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<MCMTMACPacket *>(obj)!=nullptr;
}

const char **MCMTMACPacketDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *MCMTMACPacketDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int MCMTMACPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 16+basedesc->getFieldCount() : 16;
}

unsigned int MCMTMACPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<16) ? fieldTypeFlags[field] : 0;
}

const char *MCMTMACPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "MCMTMACPacketType",
        "hasEncapsulatedPacket",
        "isBlankChainAssign",
        "encapsulatedNodeId",
        "encapsulatedParentId",
        "encapsulatedCHId",
        "encapsulatedTransmissionCount",
        "encapsulatedGeocolor",
        "parentId",
        "CHId",
        "sourceGeoColor",
        "transmissionCount",
        "batchClaimTimeslots",
        "batchClaimChannelIds",
        "singleClaimTimeslot",
        "singleChannelId",
    };
    return (field>=0 && field<16) ? fieldNames[field] : nullptr;
}

int MCMTMACPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='M' && strcmp(fieldName, "MCMTMACPacketType")==0) return base+0;
    if (fieldName[0]=='h' && strcmp(fieldName, "hasEncapsulatedPacket")==0) return base+1;
    if (fieldName[0]=='i' && strcmp(fieldName, "isBlankChainAssign")==0) return base+2;
    if (fieldName[0]=='e' && strcmp(fieldName, "encapsulatedNodeId")==0) return base+3;
    if (fieldName[0]=='e' && strcmp(fieldName, "encapsulatedParentId")==0) return base+4;
    if (fieldName[0]=='e' && strcmp(fieldName, "encapsulatedCHId")==0) return base+5;
    if (fieldName[0]=='e' && strcmp(fieldName, "encapsulatedTransmissionCount")==0) return base+6;
    if (fieldName[0]=='e' && strcmp(fieldName, "encapsulatedGeocolor")==0) return base+7;
    if (fieldName[0]=='p' && strcmp(fieldName, "parentId")==0) return base+8;
    if (fieldName[0]=='C' && strcmp(fieldName, "CHId")==0) return base+9;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceGeoColor")==0) return base+10;
    if (fieldName[0]=='t' && strcmp(fieldName, "transmissionCount")==0) return base+11;
    if (fieldName[0]=='b' && strcmp(fieldName, "batchClaimTimeslots")==0) return base+12;
    if (fieldName[0]=='b' && strcmp(fieldName, "batchClaimChannelIds")==0) return base+13;
    if (fieldName[0]=='s' && strcmp(fieldName, "singleClaimTimeslot")==0) return base+14;
    if (fieldName[0]=='s' && strcmp(fieldName, "singleChannelId")==0) return base+15;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *MCMTMACPacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "bool",
        "bool",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<16) ? fieldTypeStrings[field] : nullptr;
}

const char **MCMTMACPacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: {
            static const char *names[] = { "enum",  nullptr };
            return names;
        }
        case 10: {
            static const char *names[] = { "enum",  nullptr };
            return names;
        }
        default: return nullptr;
    }
}

const char *MCMTMACPacketDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "MCMTMACPacket_type";
            return nullptr;
        case 10:
            if (!strcmp(propertyname,"enum")) return "MCMTMACGeoColor_type";
            return nullptr;
        default: return nullptr;
    }
}

int MCMTMACPacketDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    MCMTMACPacket *pp = (MCMTMACPacket *)object; (void)pp;
    switch (field) {
        case 12: return pp->getBatchClaimTimeslotsArraySize();
        case 13: return pp->getBatchClaimChannelIdsArraySize();
        default: return 0;
    }
}

const char *MCMTMACPacketDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    MCMTMACPacket *pp = (MCMTMACPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string MCMTMACPacketDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    MCMTMACPacket *pp = (MCMTMACPacket *)object; (void)pp;
    switch (field) {
        case 0: return enum2string(pp->getMCMTMACPacketType(), "MCMTMACPacket_type");
        case 1: return bool2string(pp->getHasEncapsulatedPacket());
        case 2: return bool2string(pp->getIsBlankChainAssign());
        case 3: return long2string(pp->getEncapsulatedNodeId());
        case 4: return long2string(pp->getEncapsulatedParentId());
        case 5: return long2string(pp->getEncapsulatedCHId());
        case 6: return long2string(pp->getEncapsulatedTransmissionCount());
        case 7: return long2string(pp->getEncapsulatedGeocolor());
        case 8: return long2string(pp->getParentId());
        case 9: return long2string(pp->getCHId());
        case 10: return enum2string(pp->getSourceGeoColor(), "MCMTMACGeoColor_type");
        case 11: return long2string(pp->getTransmissionCount());
        case 12: return long2string(pp->getBatchClaimTimeslots(i));
        case 13: return long2string(pp->getBatchClaimChannelIds(i));
        case 14: return long2string(pp->getSingleClaimTimeslot());
        case 15: return long2string(pp->getSingleChannelId());
        default: return "";
    }
}

bool MCMTMACPacketDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    MCMTMACPacket *pp = (MCMTMACPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setMCMTMACPacketType((MCMTMACPacket_type)string2enum(value, "MCMTMACPacket_type")); return true;
        case 1: pp->setHasEncapsulatedPacket(string2bool(value)); return true;
        case 2: pp->setIsBlankChainAssign(string2bool(value)); return true;
        case 3: pp->setEncapsulatedNodeId(string2long(value)); return true;
        case 4: pp->setEncapsulatedParentId(string2long(value)); return true;
        case 5: pp->setEncapsulatedCHId(string2long(value)); return true;
        case 6: pp->setEncapsulatedTransmissionCount(string2long(value)); return true;
        case 7: pp->setEncapsulatedGeocolor(string2long(value)); return true;
        case 8: pp->setParentId(string2long(value)); return true;
        case 9: pp->setCHId(string2long(value)); return true;
        case 10: pp->setSourceGeoColor((MCMTMACGeoColor_type)string2enum(value, "MCMTMACGeoColor_type")); return true;
        case 11: pp->setTransmissionCount(string2long(value)); return true;
        case 12: pp->setBatchClaimTimeslots(i,string2long(value)); return true;
        case 13: pp->setBatchClaimChannelIds(i,string2long(value)); return true;
        case 14: pp->setSingleClaimTimeslot(string2long(value)); return true;
        case 15: pp->setSingleChannelId(string2long(value)); return true;
        default: return false;
    }
}

const char *MCMTMACPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *MCMTMACPacketDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    MCMTMACPacket *pp = (MCMTMACPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


