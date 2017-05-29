#include "numbersequencetodoublelist.h"

#include <QString>
#include <QRegExp>
#include <QDebug>
#include <QStringList>

NumberSequenceToDoubleList::NumberSequenceToDoubleList( const std::string&expression )
    : _error( false )
{


    /*
   std::string s = expression;
    // delimiters are spaces (\s) and/or commas
#if GCC_VERSION >= 40900

    using std::regex;
    using std::string;
    using std::sregex_token_iterator;

    regex re("[\\s,]+");
    sregex_token_iterator it(s.begin(), s.end(), re, -1);
    sregex_token_iterator reg_end;
#else

    const char* pattern ="[\\s,]+";

    std::string text(s);
    boost::regex ip_regex(pattern);
    boost::sregex_token_iterator it(text.begin(), text.end(), ip_regex, -1);
    boost::sregex_token_iterator reg_end;

#endif

    for (; it != reg_end; ++it)
    {
        //std::cout<<"it->str()=" << it->str() << std::endl;
        ret += parse( it->str() );
    }

    if (ret<0)const
    {
        error=true;
    }
  */

    QRegExp rx("[\\s+,]");
    QString str = QString::fromStdString(expression);
    QStringList list = str.split(rx);

    //qDebug() << "split: " <<list;

    if(list.size() == 0)
    {
        _error = true;
    }

    int ret = 0;
    foreach(QString token, list )
    {
        if(token.length() > 0)
        {
            ret += parse( token.toStdString() );
        }
    }

    if (ret<0)
    {
        _error=true;
    }
}

std::vector<double> NumberSequenceToDoubleList::getStdVector()
{ 
    return sequence;
}

std::string NumberSequenceToDoubleList::getErrorString()
{
    return errstr;
}

bool NumberSequenceToDoubleList::error()
{
    return _error;
}

size_t NumberSequenceToDoubleList::size()
{
    return sequence.size();
}

int NumberSequenceToDoubleList::parse(std::string s)
{   
    // cout << "parse("<<s<<")"<<endl;
    // if we find 1 or 2 ":" treat them.
    replace( s.begin(), s.end(), ',', ' ');

    size_t ncolons = count(s.begin(), s.end(), ':');

    replace( s.begin(), s.end(), ':', ' ');
    //	cout << "ncolons=" <<  ncolons << endl;

    if( ncolons == 2)
    {
        // "start:inc:end"
        std::istringstream is(s);
        double start, inc, end;

        if( !(is >> start) ||!(is >> inc) || !(is >> end))
        {
            errstr = "General parse error in numbersequence '" + s + "'";
            return -1;
        }

        // cout << "start=" << start <<endl;
        // cout << "inc=" << inc <<endl;
        // cout << "end=" << end <<endl;

        if( (start<end) && (inc<0) )
        {
            errstr = "Parse error in numbersequence '" + s + "', start<end, but neg. increment.";
            return -1;
        }

        if( (start>end) && (inc>0) )
        {
            errstr = "Parse error  in numbersequence '" + s + "', start>end, but pos. increment.";
            return -1;
        }

        /*
        if( inc < std::numeric_limits<long double>::epsilon() )
        {
            cerr << "Parse error 3 in Numbersequnce '"<<s<<"'"<<endl;
            return;
        }
        */

        // start == end?
        if( std::fabs(end-start) < std::numeric_limits<double>::epsilon() )
        {
            sequence.push_back(start);
            return 0;
        }

        // cout << "eps = " <<  std::numeric_limits<double>::epsilon()<<endl;

        long int N = std::fabs(end-start) / std::fabs(inc);
        double val=start;
        for( long int i=0; i<=N; i++)
        {
            if( std::fabs(val) <  std::numeric_limits<double>::epsilon() )
            {
                val = 0;
            }

            sequence.push_back(val);
            val += inc;
        }
    }
    else if( ncolons == 1)
    {
        // "start:inc:end"
        replace( s.begin(), s.end(), ':', ' ');
        std::istringstream is(s);
        double start, end;
        if( !(is >> start) || !(is >> end))
        {
            errstr = "General parse error in numbersequence '" + s + "'";
            return -1;
        }

        // cout << "start=" << start <<endl;
        // cout << "end=" << end <<endl;

        // start == end?
        if( std::fabs(end-start) < std::numeric_limits<double>::epsilon() )
        {
            sequence.push_back(start);
            return 0;
        }


        if(start<end)
        {
            long int N = std::fabs(end-start);
            double val=start;
            for( long int i=0; i<=N; i++)
            {
                if( std::fabs(val) <  std::numeric_limits<double>::epsilon() )
                {
                    val = 0;
                }

                sequence.push_back(val);
                val += 1.0;
            }

        }
        else
        {
            errstr = "Parse error in numbersequence '" + s + "', start<end, but increment is +1.";
            return -1;
        }
    }
    else // no colons, plain number
    {
        std::istringstream is(s);
        double val;
        if( !(is >> val))
        {
            errstr = "Parse error in Numbersequnce '" + s + "'";
            return -1;
        }

        sequence.push_back(val);
    }

    return 0; // no error
}


