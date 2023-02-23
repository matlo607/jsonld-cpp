#ifndef LIBJSONLD_CPP_RDFQUAD_H
#define LIBJSONLD_CPP_RDFQUAD_H

#include <jsonld-cpp/RDFNode.h>
#include <iostream>
#include <memory>
#include <string>


namespace RDF {

    class RDFQuad {
    private:

        std::shared_ptr<Node> subject;
        std::shared_ptr<Node> predicate;
        std::shared_ptr<Node> object;
        std::shared_ptr<Node> graph;

        void setSubject(std::shared_ptr<Node> isubject) { subject = std::move(isubject); }
        void setPredicate(std::shared_ptr<Node> ipredicate)  { predicate = std::move(ipredicate); }
        void setObject(std::shared_ptr<Node> iobject)  { object = std::move(iobject); }
        void setGraph(std::shared_ptr<Node> igraph)  { graph = std::move(igraph); }

    public:

        RDFQuad(std::shared_ptr<Node> isubject, std::shared_ptr<Node> ipredicate, std::shared_ptr<Node> iobject,
                std::string * igraph);

        RDFQuad(const std::string& isubject, const std::string& ipredicate, const std::string& iobject, std::string * igraph);

        RDFQuad(const RDFQuad &rhs);
        RDFQuad(RDFQuad&& rhs) noexcept;

        RDFQuad& operator= (const RDFQuad &rhs);
        RDFQuad& operator= (RDFQuad&& rhs) noexcept;

        std::shared_ptr<Node> getPredicate() const;
        std::shared_ptr<Node> getObject() const;
        std::shared_ptr<Node> getSubject() const;
        std::shared_ptr<Node> getGraph() const;

        std::string toString() const;

        friend bool operator==(const RDFQuad &lhs, const RDFQuad &rhs);

    };

    bool operator==(const RDF::RDFQuad &lhs, const RDF::RDFQuad &rhs);
    bool operator!=(const RDF::RDFQuad &lhs, const RDF::RDFQuad &rhs);
    bool operator<(const RDFQuad &lhs, const RDFQuad &rhs);
    bool operator>(const RDFQuad &lhs, const RDFQuad &rhs);
    bool operator<=(const RDFQuad &lhs, const RDFQuad &rhs);
    bool operator>=(const RDFQuad &lhs, const RDFQuad &rhs);

    struct QuadPtrLess :
            public std::binary_function<const std::shared_ptr<RDFQuad>, const std::shared_ptr<RDFQuad>, bool> {
        bool operator()(const std::shared_ptr<RDFQuad> & lhs, const std::shared_ptr<RDFQuad> & rhs) const {
            return *lhs < *rhs ;
        }
    };

    std::ostream& operator<<(std::ostream&, const RDFQuad&);
}

namespace std {
    string to_string(const RDF::RDFQuad&);
} // namespace std

#endif //LIBJSONLD_CPP_RDFQUAD_H
