//----------------------------------------------------------------------------
// Name: swirl-networking.cpp
// Desc: Networking and OSC stuff
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//----------------------------------------------------------------------------

#include "swirl-networking.h"
#include <pthread.h> //TODO necessary?
#include "swirl-globals.h"
#include "UdpSocket.h" //TODO
#include "OscReceivedElements.h"
#include "OscPacketListener.h"
#include "OscOutboundPacketStream.h"

using namespace std;

struct NetworkLocation
{
    const char address[15];
    int port;
};

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
            else if( std::strcmp( m.AddressPattern(), "/rotated" ) == 0 ){
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
            else if( std::strcmp( m.AddressPattern(), "/moveForward" ) == 0 ){
                // example #1 -- argument stream interface
                osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
                float f;

                std::cout << "RECEVING \n";

                args >> f >> osc::EndMessage;

                std::cout << f << std::endl;
                
                //TODO  Vector3D lookVector = Globals::cameraReference - Globals::cameraEye;
                Vector3D lookVector = Globals::swirlCamera.reference - Globals::swirlCamera.eye;
                
                 Vector3D movementVector = lookVector;
                 movementVector.normalize();
                 movementVector *= -0.1;

                 //TODO Globals::cameraReference += movementVector;
                Globals::swirlCamera.reference += movementVector;
                
                //TODO Globals::cameraEye += movementVector;
                Globals::swirlCamera.eye += movementVector;
   
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


//-----------------------------------------------------------------------------
// Name: oscListener()
// Desc: ...
//-----------------------------------------------------------------------------
void * oscListener(void * args)
{
    cerr << "Starting listening thread!" << endl;
    ExamplePacketListener listener;

    NetworkLocation *  receiveLocation = (NetworkLocation*)args;

    UdpListeningReceiveSocket s(
        IpEndpointName(
          //IpEndpointName::ANY_ADDRESS,
          receiveLocation->address,
          receiveLocation->port
          ),
        &listener
        );

    //s.RunUntilSigInt();
    s.Run();

    pthread_exit(NULL);
    cerr << "Exiting thread!" << endl;
}

//TODO
//-----------------------------------------------------------------------------
// Name: transmitSocket
// Desc: ...
//-----------------------------------------------------------------------------
/*static*/ UdpTransmitSocket* transmitSocket = NULL;

//TODO
//-----------------------------------------------------------------------------
// Name: getTransmitSocket()
// Desc: ...
//-----------------------------------------------------------------------------
UdpTransmitSocket* getTransmitSocket()
{
    return transmitSocket;
}

//TODO
//-----------------------------------------------------------------------------
// Name: getTransmitSocket(const char* address, int port)
// Desc: ...
//-----------------------------------------------------------------------------
UdpTransmitSocket* getTransmitSocket(const char* address, int port)
{
    //TODO
    if (!transmitSocket)
        transmitSocket = new UdpTransmitSocket( IpEndpointName(address, port) );
    return transmitSocket;

}

//-----------------------------------------------------------------------------
// Name: swirl_send_message(char* label, float value)
// Desc: send an osc message
//-----------------------------------------------------------------------------
void * swirl_send_message(const char* label, float value)
{
   UdpTransmitSocket* transmitSocket = getTransmitSocket();

   char buffer[SWIRL_FRAMESIZE];
   osc::OutboundPacketStream oscOutstream( buffer, SWIRL_FRAMESIZE);

   oscOutstream
     << osc::BeginBundleImmediate
     << osc::BeginMessage( label )
     << value
     << osc::EndMessage
     << osc::EndBundle;

   transmitSocket->Send( oscOutstream.Data(), oscOutstream.Size() );
}

