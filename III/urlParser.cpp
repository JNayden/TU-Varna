
#include <iostream>

bool validSymbols(char ch)
{
    return (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch >= '0' && ch <= '9' || ch == '-');
}
bool validDomain(std::string const& str, static int j) 
{
    if (validSymbols(str.at(j)) && j < str.length() - 1)
    {
        return validDomain(str, j+=1);
    }
}
bool isAlphabet(char ch)
{
    return (ch >= 'a' && ch <= 'z');
}
bool validTLD(std::string  str) 
{
    if (isAlphabet(str.at(0)))
    {
        int size = str.substr(0, str.length()).length();
        if (isAlphabet(str.at(str.length() - size--)))
        {
            str = str.substr(str.find(str.at(0)) + 1, str.length());
            if (str == "")
                return true;
            return validTLD(str);
        }
    }
    return false;
}
bool validPathSymbols(char ch)
{
    return (ch >= 'a' && ch <= 'z' || ch >= '@' && ch <= 'Z' || ch >= '0' && ch <= ';' || ch == '-' ||
        ch >= '!' && ch <=  '$' || ch >= '&' && ch <= '/' || ch == '\_' || ch == '\\');
}
bool validPath(std::string  str)
{
    if (validPathSymbols(str.at(0)))
    {
        int size = str.substr(0, str.length()).length();
        if (validPathSymbols(str.at(str.length() - size--)))
        {
            str = str.substr(str.find(str.at(0)) + 1, str.length());
            if (str == "")
                return true;
            return validPath(str);
        }
    }
    return false;
}
bool isHostport(std::string domain);
bool validAccount(std::string str)
{
    if (str.find(":") != std::string::npos)
    {
        int size = str.substr(0, str.find(':')).length();
        
        
        if (validPathSymbols(str.at(str.length() - size--)))
        {
            str = str.substr(str.find(str.at(0)) + 1, str.length());
            if (size == 0)
            {
                return validAccount(str.substr(str.find(":") + 1, str.length()));
            }
            return validAccount(str);
        }
    }
    else
    {
        int size = str.substr(0, str.length()).length();
        if (validPathSymbols(str.at(str.length() - size--)))
        {
            str = str.substr(str.find(str.at(0)) + 1, str.length());
            if (str == "")
                return true;
            return validAccount(str);
        }
    }
    return false;
}
bool validLogin(std::string str)
{
    if (str.find("@") != std::string::npos)
    {
        int size = str.substr(0, str.find('@')).length();
        if (size == 0)
            return false;
        if (validPathSymbols(str.at(str.length() - size--)))
        {
            str = str.substr(str.find(str.at(0)) + 1, str.length());
            if (size == 0)
            {
                return validLogin(str.substr(str.find("@") + 1, str.length()));
            }
            return validLogin(str);
        }
    }
    else
    {
        return isHostport(str);
    }
    return false;
}
bool validHex(char ch) {
    return ( ch >= 'a' && ch <= 'f'|| ch >= 'A' && ch <= 'F' || ch >= '0' && ch <= '9');
}
bool isANumber(char ch)
{
    return (ch >= '0' && ch <= '9');
}
bool validMultipleHEX(std::string path)
{
    if (path.find("%") != std::string::npos)
    {
        
        int index1 = path.find("%") + 1;
        int index2 = path.find("%") + 2;

        if (index1 < path.length() && index2 < path.length())
        {
            if (validHex(path.at(index1)) && validHex(path.at(index2)))
            {
                path = path.substr(path.find("%") + 1, path.length() );
                return validMultipleHEX(path);
            }
            return false;
        }
        else
        {
            std::cout << "\nHex is Not Full: " << "\n";
            return false;
        }
    }
}

bool isHostport(std::string domain)
{
    if (domain.find(".") != std::string::npos)
    {
        int size = domain.substr(0, domain.find(".")).length();
        if (size > 3)
            return false;
        if (isANumber(domain.at(domain.find(".") - size--)))
        {
            domain = domain.substr(domain.find(domain.at(0)) + 1, domain.length());
            if (size == 0)
            {
                return isHostport(domain.substr(domain.find(".") + 1, domain.length()));
            }
            return isHostport(domain);
        }
    }
    else
    {
        int size = domain.substr(0, domain.length()).length();
        if (size > 3)
            return false;
        if (isANumber(domain.at(domain.length() - size--)))
        {
            domain = domain.substr(domain.find(domain.at(0)) + 1, domain.length());
            if (domain == "")
                return true;
            return isHostport(domain);
        }
    }
    return false;
}

void pathCheck(std::string domain, std::string path)
{
    if (domain.find("/") == std::string::npos) // -1
    {
        return;
    }

    path = domain.substr(domain.find("/"), domain.length());
    if (validPath(path))
    {
        std::cout << "Path !%: " << path << "\n" << "\n";
        return;
    }
    if (validMultipleHEX(path))
    {
        std::cout << "Path %: " << path << "\n" << "\n";
    }
    else
    {
        std::cout << "\nHex is NOT VALID: " << "\n";
    }
}
bool hostPortValidation(std::string hostport)
{
    int i = 0;
    int occurrences = 0;
    std::string::size_type pos = 0;
    std::string target = ".";

    while ((pos = hostport.find(target, pos)) != std::string::npos) 
    {
        ++occurrences;
        pos += target.length();
        if (isdigit(hostport.at(i)))
        {
            if (occurrences > 1 && hostport.find(hostport.at(i), pos) && i < hostport.length() - 1)
            {
                return false;
            }
        }
        i++;
    }
    
}
void checkURL(std::string url)
{
    std::string path = "";
    int i = 0;
    static int j = 0;
    std::string hostport = "";

    while(1)
    {
        if (i == url.length())
        {
            break;
        }
        if (url.substr(0, 7) == "http://" ||
            url.substr(0, 6) == "ftp://")
        {
            std::cout << "Protocol: " << url.substr(0, url.find('/') + 2) << "\n";
            std::string domainToEnd = url.substr(url.find('/') + 2, std::string::npos);
            std::string domainPlusTLD = domainToEnd.substr(0, domainToEnd.find("/"));

            if (domainToEnd.find(":") != std::string::npos)
                hostport = domainToEnd.substr(0, domainToEnd.find(":"));
            else
                hostport = domainToEnd.substr(0, domainToEnd.find("/"));
                
            std::string domain = domainPlusTLD.substr(0, domainPlusTLD.find("."));
            
            
            if (validDomain(domain, j) && domain.at(0) != '-' && domain.at(domain.length() - 1) != '-' && domain.length() >= 2 && domain.length() <= 63)
            {
                if (isHostport(hostport))
                {
                    std::cout << "\nHostport: " << hostport << "\n";
                    if (domainToEnd.find(":") != std::string::npos)
                    {
                        std::string port = domainToEnd.substr(domainToEnd.find(":"), 3);
                        if (domainToEnd.substr(domainToEnd.find(":"), 3) == ":80")
                            std::cout << "\nPort - " << port << std::endl;
                    }
                    pathCheck(domainToEnd, path);
                }
                else if (domainPlusTLD.find(".") != std::string::npos && hostPortValidation(hostport))
                {
                    std::cout << "Domain: " << domain << std::endl;
                    std::string tldWithPort = domainPlusTLD.substr(domainPlusTLD.find(".") + 1, domainPlusTLD.length() - 1);
                    std::string tld = tldWithPort.substr(0, tldWithPort.find(":"));
                
                    if (validTLD(tld))
                    {
                        if (domainToEnd.find(":") != std::string::npos)
                        {
                            std::string port = domainToEnd.substr(domainToEnd.find(":"), 3);
                            if (domainToEnd.substr(domainToEnd.find(":"), 3) == ":80")
                            {
                                std::cout << "Tld: " << tld << std::endl;
                                std::cout << "Port - " << port << std::endl;
                            }
                        }
                        else
                        {
                            std::cout << "Tld: " << tld << std::endl;
                        }
                        pathCheck(domainToEnd, path);
                    }
                    else
                    {
                        std::cout << "\nTLD is not VALID\n";
                    }
                }
                else
                {
                    std::cout << "\nDomain or Hostport is NOT VALID\n";
                }
            }
            else if (domainPlusTLD.find("@") != std::string::npos)
            {
                std::string account = domainPlusTLD.substr(0, domainPlusTLD.find("@"));
                if (validLogin(domainPlusTLD))
                {
                    std::cout << "\nLogin: " << domainPlusTLD << std::endl;

                    if (validAccount(account) && domainPlusTLD.find(":") != std::string::npos)
                    {
                        std::cout << "\nAccount: " << account << std::endl;
                        pathCheck(domainToEnd, path);
                    }
                    else
                    {
                        std::cout << "\nAccount or Hostport is NOT VALID\n";
                    }
                }
                else
                {
                    std::cout << "\nAccount or Hostport is NOT VALID\n";
                }
            }
            else
            {
                std::cout << "\nDomain is not found\n" << "\n";
            }
            break;
        }
        else
        {
            std::cout << "\nProvide http\n";
        }
        break;
        i++;
    }
}

int main()
{
    std::string noProto = "htp://do-ma-in.com/path%20%2d%3c%2f%4b.html";
    std::string urlHttp = "http://do-ma-in.com/path%20%2d%3c%2f%4b.html";
    std::string wrong = "http://do-ma-in.com/path%0%2d%3c%2f%4b.html";
    std::string urlHttp1 = "http://do-main.com:80/path%2d%2d%2d%2d%2d.html";
    std::string urlHttp3 = "http://127.30.0.1/path%2d%2d%2d%2d%2d.html";
    std::string wrong2 = "http://127.1234.0.1:80/path%2d%2d%2d%2d%2d.html";
    std::string withoutPath = "http://ascii-code.com";

    std::string urlFtp = "ftp://user1!a:pass1w!ord@121.0.0.1/path";
    std::string urlFtp1 = "ftp://user1!apass1w!ord@121.0.0.1/path";
    std::string urlFtp2 = "ftp://user1!:apass1w!ord121.0.0.1/path";

    checkURL(noProto);
    std::cout << "--------------------------------------------------------------" << std::endl;
    checkURL(urlHttp);
    std::cout << "--------------------------------------------------------------" << std::endl;
    checkURL(wrong);
    std::cout << "--------------------------------------------------------------" << std::endl;
    checkURL(urlHttp1);
    std::cout << "--------------------------------------------------------------" << std::endl;
    checkURL(urlHttp3);
    std::cout << "--------------------------------------------------------------" << std::endl;
    checkURL(wrong2);
    std::cout << "--------------------------------------------------------------" << std::endl;
    checkURL(withoutPath);
    std::cout << "--------------------------------------------------------------" << std::endl;
    checkURL(urlFtp);
    std::cout << "--------------------------------------------------------------" << std::endl;
    checkURL(urlFtp1);
    std::cout << "--------------------------------------------------------------" << std::endl;
    checkURL(urlFtp2);
}
