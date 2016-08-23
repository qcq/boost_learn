#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <assert.h>
#include <iostream>
#include <boost/spirit/include/phoenix_bind.hpp>

struct date {
    unsigned short year;
    unsigned short month;
    unsigned short day;  
};

std::ostream& operator << (std::ostream & out, struct date const& date){
    out << date.year << "," << date.month << "," << date.day << std::endl;
    return out;
}

date parse_date_time1(const std::string& s) {
    using boost::spirit::qi::_1;
    using boost::spirit::qi::ushort_;
    using boost::spirit::qi::char_;
    using boost::phoenix::ref;
    
    date res;
    const char * first = s.data();
    const char * end = first + s.size();
    bool success = boost::spirit::qi::parse(first, end, 
        ushort_[ref(res.year) = _1] >> char('-')
        >> ushort_[ref(res.month) = _1] >> char('-')
        >> ushort_[ref(res.day) = _1]);
    if (!success || first != end) {
        throw std::logic_error("Parsing failed");
    } 
    return res;
}

date parse_date_time2(const std::string& s) {
    using boost::spirit::qi::_1;
    using boost::spirit::qi::uint_parser;
    using boost::spirit::qi::char_;
    using boost::phoenix::ref;
    
    uint_parser<unsigned short, 10, 2, 2> u2_;
    uint_parser<unsigned short, 10, 4, 4> u4_;
    
    date res;
    const char * first = s.data();
    const char * end = first + s.size();
    bool success = boost::spirit::qi::parse(first, end, 
        u4_[ref(res.year) = _1] >> char_('-')
        >> u2_[ref(res.month) = _1] >> char_('-')
        >> u2_[ref(res.day) = _1]);
    if (!success || first != end) {
        throw std::logic_error("Parsing failed");
    } 
    return res;
}

struct datetime {
    enum zone_offset_t {
        OFFSET_NO_SET,
        OFFSET_Z,
        OFFSET_UTC_PLUS,
        OFFSET_UTC_MINUS,
    };
private:
    unsigned short year_;
    unsigned short month_;
    unsigned short day_;
    unsigned short hours_;
    unsigned short minutes_;
    unsigned short seconds_;
    zone_offset_t zone_offset_type_;
    unsigned int zone_offset_in_min_;
    
    static void dt_assert(bool v, const char * msg) {
        if (!v) {
            throw std::logic_error("Assertion error: " + std::string(msg));
        }
    }    
public:
    datetime():year_(0), month_(0), day_(0), hours_(0), minutes_(0), seconds_(0), zone_offset_type_(OFFSET_NO_SET), 
        zone_offset_in_min_(0) {}

    void set_year(unsigned short val) {
        year_ = val;
    }

    void set_month(unsigned short val) {
        month_ = val;
    }

    void set_day(unsigned short val) {
        day_ = val;
    }

    void set_hours(unsigned short val) {
        hours_ = val;
    }

    void set_minutes(unsigned short val) {
        minutes_ = val;
    }

    void set_seconds(unsigned short val) {
        seconds_ = val;
    }

    void set_zone_offset_type(zone_offset_t val) {
        zone_offset_type_ = val;
    }

    void set_zone_offset_in_min(unsigned int val) {
        zone_offset_in_min_ = val;
    }
    friend std::ostream& operator << (std::ostream & out, struct datetime const& date);
};

std::ostream& operator << (std::ostream & out, struct datetime const& date){
    out << date.year_ << "," << date.month_ << "," << date.day_ << ","
    << date.hours_ << "," << date.minutes_ << "," << date.seconds_ << ","
     << date.zone_offset_in_min_ << ","  << date.zone_offset_type_ << std::endl;
    return out;
}

void set_zone_offset(datetime& dt, char sign, unsigned short hours, unsigned short minutes) {
    dt.set_zone_offset_type(sign == '+' ? datetime::OFFSET_UTC_PLUS : datetime::OFFSET_UTC_MINUS);
    dt.set_zone_offset_in_min(hours * 60 + minutes);
}
datetime parse_datetime(const std::string& s) {
    using boost::spirit::qi::_1;
    using boost::spirit::qi::_2;
    using boost::spirit::qi::_3;

    using boost::spirit::qi::uint_parser;
    using boost::spirit::qi::char_;
    using boost::phoenix::ref;
    using boost::phoenix::bind;
    datetime ret;
    uint_parser<unsigned short, 10, 2, 2> u2_;
    uint_parser<unsigned short, 10, 4, 4> u4_;
    boost::spirit::qi::rule<const char *, void()> timezone_parser
        = -(
            char_('Z')[bind(&datetime::set_zone_offset_type, &ret, datetime::OFFSET_Z)] |
            ((char_('+') | char_('-')) >> u2_ >> ':' >> u2_)[bind(&set_zone_offset, ref(ret), _1, _2, _3)]
        );
    boost::spirit::qi::rule<const char * , void()> date_parser = 
        u4_[bind(&datetime::set_year, &ret, _1)] >> char_('-') >> u2_[bind(&datetime::set_month, &ret, _1)]
        >> char_('-') >> u2_[bind(&datetime::set_day, &ret, _1)];

    boost::spirit::qi::rule<const char *, void()> time_parser = 
        u2_[bind(&datetime::set_hours, &ret, _1)] >> char_(':')
        >> u2_[bind(&datetime::set_minutes, &ret, _1)] >> char_(':')
        >> u2_[bind(&datetime::set_seconds, &ret, _1)];

    const char * first = s.data();
    const char * end = first + s.size();
    bool success = boost::spirit::qi::parse(first, end, 
        ((date_parser >> char_('T') >> time_parser) | date_parser | time_parser) >> timezone_parser);
    if (!success || first != end) {
        throw std::logic_error("Parsing failed");
    } 
    return ret;
}

int main()
{
    date d = parse_date_time2("2012-12-31");
    std::cout << parse_datetime("2016-12-20T11:00:00+09:15");
    std::cout << d << std::endl;
    return 0;
}