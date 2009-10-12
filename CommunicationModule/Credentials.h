#ifndef incl_Comm_Credentials_h
#define incl_Comm_Credentials_h

#include "Foundation.h"
#include "EventDataInterface.h"
#include "interface.h"


namespace TpQt4Communication
{

	/**
	 *  Credential for connecting to IM server
	 *
	 *  NOTE: Propably attribute map with arbitrary keys value pairs are the best choise here...
	 *
	 */
	class Credentials
	{
	public:
		Credentials();
		void SetProtocol(std::string protocol);
		void SetUserID(std::string user_id);
		void SetPassword(std::string pwd);
		void SetServer(std::string server);
		void SetServerPort(int);
		std::string GetProtocol() const;
		std::string GetUserID() const ;
		std::string GetUserPassword() const;
		std::string GetServer() const;
		int GetServerPort() const;
		void LoadFromFile(std::string path);
	private:
		std::string user_id_;
		std::string password_;
		std::string protocol_;
		std::string server_;
		int port_;
	//	std::map<std::string, std::string> attributes_;
	};
	typedef boost::weak_ptr<Credentials> CredentialsWeakPtr;
	

} // end of namespace: TpQt4Communication


namespace Communication
{
	/**
	 * 
	 *
	 *
	 */
	class Credentials : public CredentialsInterface
	{
	public:
		Credentials(const QString& protocol, const QString& user_id, const QString &server, int port );

		//! Define the used protocol eg. "Jabber", "Opensim_UDP_IM", "irc"
		virtual void SetProtocol(const QString &protocol);

		//! Provides the protocol
		virtual QString GetProtocol() const;

		virtual void SetUserID(const QString &user_id);
		virtual QString GetUserID() const;

		virtual void SetServer(const QString &server);
		virtual QString GetServer() const;

		//! Define port of IM server to connect
		virtual void SetPort(int port);

		//! Provides the port of IM server
		virtual int GetPort() const;
	protected:
		QString protocol_;
		QString user_id_;
		QString server_;
		int port_;
	};
}

#endif // incl_Comm_Credentials_h
