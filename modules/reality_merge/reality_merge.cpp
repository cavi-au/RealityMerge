// local
#include "reality_merge.h"


// RMDocument
void RMDocument::_bind_methods() {
    ClassDB::bind_method(D_METHOD("to_string"), &RMDocument::to_string);
}


String RMDocument::to_string() {
    if (!m_document) {
        return String{""};
    } else {
        return String{"<document-id>"};
    }
}


bool operator< (RMDocument const& lhs, RMDocument const& rhs) {
    return lhs.m_document < rhs.m_document;
}


bool operator==(RMDocument const& lhs, RMDocument const& rhs) {
    return (lhs.m_document == rhs.m_document) || AMequal(lhs.m_document, rhs.m_document);
}

