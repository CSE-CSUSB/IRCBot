#ifndef IRC_BOT_HH
#define IRC_BOT_HH
#include "irc_message.hh"
#include "socketstream.hh"
#include "net_utility.hh"
#include <iostream>
#include <string>
#include <set>

class irc_bot {
private:
	std::string nickname_, owner_, node_, service_;
	std::set<std::string> channels_;
	socketstream stream_;
public:
	irc_bot() = default;
	irc_bot(const irc_bot&) = delete;
	irc_bot(irc_bot&&) = default;
	irc_bot& operator=(const irc_bot&) = delete;
	irc_bot& operator=(irc_bot&&) = default;

	bool operator!() const
	{
		return stream_.operator!();
	}

	operator bool() const
	{
		return stream_.operator bool();
	}

	const std::set<std::string>& channels() const
	{
		return channels_;
	}

	void connect(const std::string& node, const std::string& service,
						const std::string& pass = "");
	void join(const std::string& channel);
	std::set<std::string> names(const std::string& channel);

	std::string nickname() const
	{
		return nickname_;
	}

	void nickname(const std::string& set_nickname);

	std::string node() const
	{
		return node_;
	}

	std::string owner() const
	{
		return owner_;
	}

	void owner(const std::string& set_owner)
	{
		owner_ = set_owner;
	}

	void part(const std::string& channel, const std::string& message = "");
	void pong(const std::string& id);
	void privmsg(const std::string& target, const std::string& message);
	irc_bot& readmsg(irc_message& msg);

	std::string service() const
	{
		return service_;
	}

	socketstream& stream()
	{
		return stream_;
	}

	void quit(const std::string& message = "");
};

#include "irc_bot.icc"

#endif
