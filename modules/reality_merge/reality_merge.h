#ifndef REALITY_MERGE_H
#define REALITY_MERGE_H

// third-party
extern "C" {
    
#include <automerge-c/automerge.h>

}

// regional
#include <core/io/resource.h>
#include <core/string/ustring.h>


class RMDocument : public Resource {
    GDCLASS(RMDocument, Resource);

public:
    RMDocument();

    operator AMdoc*() const;
    
protected:
    static void _bind_methods();

    String to_string();

private:
    AMdoc* m_document;

    friend bool operator< (RMDocument const& lhs, RMDocument const& rhs);

    friend bool operator==(RMDocument const& lhs, RMDocument const& rhs);

    friend bool operator!=(RMDocument const& lhs, RMDocument const& rhs);

};


_FORCE_INLINE_ RMDocument::RMDocument()
    :  m_document{nullptr} {
}


_FORCE_INLINE_ RMDocument::operator AMdoc*() const {
    return m_document;
}
    

_FORCE_INLINE_ bool operator!=(RMDocument const& lhs, RMDocument const& rhs) {
    return !operator==(lhs, rhs);
}

#endif // REALITY_MERGE_H
