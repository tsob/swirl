//----------------------------------------------------------------------------
// Name: swirl-networking.h
// Desc: Networking and OSC stuff
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//----------------------------------------------------------------------------

#include "swirl-globals.h"
#include "UdpSocket.h" //TODO
#include "OscReceivedElements.h"
#include "OscPacketListener.h"
#include "OscOutboundPacketStream.h"

#define ADDRESS "127.0.0.1"
#define PORT 7000

//-----------------------------------------------------------------------------
// Function declarations
//-----------------------------------------------------------------------------
bool swirl_networking_init( int argc, const char ** argv );
void * oscListener(void * args);

UdpTransmitSocket* getTransmitSocket();
UdpTransmitSocket* getTransmitSocket(const char* address, int port);

//-----------------------------------------------------------------------------
// Name: class ExamplePacketListener
// Desc: our packet listener
//-----------------------------------------------------------------------------
class ExamplePacketListener : public osc::OscPacketListener {
protected:

    virtual void ProcessMessage( const osc::ReceivedMessage& m,
                                const IpEndpointName& remoteEndpoint )
    {
        (void) remoteEndpoint; // suppress unused parameter warning

        try{
            // example of parsing single messages. osc::OsckPacketListener
            // handles the bundle traversal.

            /*if( std::strcmp( m.AddressPattern(), "/cameraReferenceZ" ) == 0 ){
                // example #1 -- argument stream interface
                osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
                float f;
                args >> f >> osc::EndMessage;

                std::cout << "received '/cameraReferenceZ' message with arguments: "
                << f << "\n";

                //TODO
                getAvatar()->loc.x = f;

            } else */ if( std::strcmp( m.AddressPattern(), "/cameraEyeZ" ) == 0 ){
                // example #1 -- argument stream interface
                osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
                float f;

                std::cout << "RECEVING \n";

                args >> f >> osc::EndMessage;

                std::cout << f << std::endl;

                getAvatar()->loc.z = f;

               // getAvatar()->loc.y = f;


                std::cout << "received '/cameraEyeZ' message with arguments: "
                << f << "\n";
            }
            else if( std::strcmp( m.AddressPattern(), "/cameraEyeX" ) == 0 ){
                // example #1 -- argument stream interface
                osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
                float f;

                std::cout << "RECEVING \n";

                args >> f >> osc::EndMessage;

                std::cout << f << std::endl;

                getAvatar()->loc.x = f;

                // getAvatar()->loc.y = f;


                std::cout << "received '/cameraEyeZ' message with arguments: "
                << f << "\n";
            }
            else if( std::strcmp( m.AddressPattern(), "/cameraReferenceX" ) == 0 ){
                // example #1 -- argument stream interface
                osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
                float f;

                std::cout << "RECEVING \n";

                args >> f >> osc::EndMessage;

                std::cout << f << std::endl;

                getAvatar()->ori.y += (180.0f / ONE_PI * f);


                // getAvatar()->loc.y = f;


                std::cout << "received '/cameraEyeZ' message with arguments: "
                << f << "\n";
            }
            else if( std::strcmp( m.AddressPattern(), "/cameraReferenceZ" ) == 0 ){
                // example #1 -- argument stream interface
                osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
                float f;

                std::cout << "RECEVING \n";

                args >> f >> osc::EndMessage;

                std::cout << f << std::endl;

                getAvatar()->ori.y += (180 / (3.14f) * f);

                // getAvatar()->loc.y = f;


                std::cout << "received '/cameraEyeZ' message with arguments: "
                << f << "\n";
            }

        }catch( osc::Exception& e ){
            // any parsing errors such as unexpected argument types, or
            // missing arguments get thrown as exceptions.
            std::cout << "error while parsing message: "
            << m.AddressPattern() << ": " << e.what() << "\n";
        }
    }
};
