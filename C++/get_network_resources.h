#include <Windows.h>
#include <vector>
#pragma comment(lib, "Mpr.lib")

// Class used to represent a network resource
class NetworkResource final{
public:

	// Enumerator used to represent the type of a network resource
	enum class Type : unsigned long int{
		Invalid = 0x00000000,
		Disk = RESOURCETYPE_DISK,
		Print = RESOURCETYPE_PRINT,
		Any = RESOURCETYPE_ANY
	};

	// Enumerator used to represent the display type of a network resource
	enum class DisplayType : unsigned long int{
		Invalid = 0x00000000,
		Network = RESOURCEDISPLAYTYPE_NETWORK,
		Domain = RESOURCEDISPLAYTYPE_DOMAIN,
		Server = RESOURCEDISPLAYTYPE_SERVER,
		Share = RESOURCEDISPLAYTYPE_SHARE,
		Directory = RESOURCEDISPLAYTYPE_DIRECTORY,
		Generic = RESOURCEDISPLAYTYPE_GENERIC
	};

	// Enumerator used to represent the usage of a network resource
	enum class Usage : unsigned long int{
		Invalid = 0x00000000,
		Connectable = RESOURCEUSAGE_CONNECTABLE,
		Container = RESOURCEUSAGE_CONTAINER
	};

	// Enumerator used to represent the scope of a network resource
	enum class Scope : unsigned long int{
		Invalid = 0x00000000,
		Connected = RESOURCE_CONNECTED,
		GlobalNetwork = RESOURCE_GLOBALNET,
		Context = RESOURCE_CONTEXT
	};
private:

	// Initializes all strings
	void __InitializeStrings(
		const char* const _local_name,
		const char* const _remote_name,
		const char* const _provider_name,
		const char* const _comment
	){

		// Get length of all strings
		const unsigned int local_name_len = _local_name ? strlen(_local_name) : 0;
		const unsigned int remote_name_len = _remote_name ? strlen(_remote_name) : 0;
		const unsigned int provider_name_len = _provider_name ? strlen(_provider_name) : 0;
		const unsigned int comment_len = _comment ? strlen(_comment) : 0;

		// Allocate a single block of memory that fits all strings including their null terminating characters
		char* const str_net_rsc = new char[static_cast<unsigned long long int>(local_name_len) + remote_name_len + provider_name_len + comment_len + 4]{};
		unsigned int last_str_indx = 0;
		if(local_name_len){
			m_local_name = &str_net_rsc[last_str_indx];
			_memccpy(m_local_name, _local_name, 0, local_name_len);
			last_str_indx += local_name_len + 1;
		}
		if(remote_name_len){
			m_remote_name = &str_net_rsc[last_str_indx];
			_memccpy(m_remote_name, _remote_name, 0, remote_name_len);
			last_str_indx += remote_name_len + 1;
		}
		if(provider_name_len){
			m_provider_name = &str_net_rsc[last_str_indx];
			_memccpy(m_provider_name, _provider_name, 0, provider_name_len);
			last_str_indx += provider_name_len + 1;
		}
		if(comment_len){
			m_comment = &str_net_rsc[last_str_indx];
			_memccpy(m_comment, _comment, 0, provider_name_len);
			last_str_indx += provider_name_len + 1;
		}
	}

	// Frees the memory used to allocate all strings
	void __FreeStrings() const{
		if(m_local_name){
			delete[] m_local_name;
		}
		else if(m_remote_name){
			delete[] m_remote_name;
		}
		else if(m_provider_name){
			delete[] m_provider_name;
		}
		else if(m_comment){
			delete[] m_comment;
		}
	}

	Type m_type = Type::Invalid;
	DisplayType m_display_type = DisplayType::Invalid;
	Usage m_usage = Usage::Invalid;
	Scope m_scope = Scope::Invalid;
	char* m_local_name = nullptr;
	char* m_remote_name = nullptr;
	char* m_provider_name = nullptr;
	char* m_comment = nullptr;
public:

	// Copy constructor
	NetworkResource(const NetworkResource& _net_rsc) :
		m_type(_net_rsc.m_type),
		m_display_type(_net_rsc.m_display_type),
		m_usage(_net_rsc.m_usage),
		m_scope(_net_rsc.m_scope)
	{
		__InitializeStrings(_net_rsc.m_local_name, _net_rsc.m_remote_name, _net_rsc.m_provider_name, _net_rsc.m_comment);
	}

	// NETRESOURCEA constructor
	NetworkResource(const NETRESOURCEA& _net_rsc) :
		m_type(static_cast<Type>(_net_rsc.dwType)),
		m_display_type(static_cast<DisplayType>(_net_rsc.dwDisplayType)),
		m_usage(static_cast<Usage>(_net_rsc.dwUsage)),
		m_scope(static_cast<Scope>(_net_rsc.dwScope))
	{
		__InitializeStrings(_net_rsc.lpLocalName, _net_rsc.lpRemoteName, _net_rsc.lpProvider, _net_rsc.lpComment);
	}

	~NetworkResource(){
		__FreeStrings();
	}

	// Returns the type of the current network resource
	inline const Type& GetType() const noexcept{
		return m_type;
	}

	// Returns the display type of the current network resource
	inline const DisplayType& GetDisplayType() const noexcept{
		return m_display_type;
	}

	// Returns the usage of the current network resource
	inline const Usage& GetUsage() const noexcept{
		return m_usage;
	}

	// Returns the scope of the current network resource
	inline const Scope& GetScope() const noexcept{
		return m_scope;
	}

	// Returns the local name of the current network resource
	inline const char* const GetLocalName() const noexcept{
		return m_local_name;
	}

	// Returns the remote name of the current network resource
	inline const char* const GetRemoteName() const noexcept{
		return m_remote_name;
	}

	// Returns the provider name of the current network resource
	inline const char* const GetProviderName() const noexcept{
		return m_provider_name;
	}

	// Returns the comment of the current network resource
	inline const char* const GetCommentName() const noexcept{
		return m_comment;
	}
};

// Returns a list with all of the network resources found
static const std::vector<NetworkResource> GetNetworkResources(){
	std::vector<NetworkResource> network_resources = std::vector<NetworkResource>();

	// Get network resource enumerator
	NETRESOURCEA net_rsc = NETRESOURCEA();
	HANDLE net_rsc_enum = HANDLE();
	if(WNetOpenEnumA(RESOURCE_GLOBALNET, RESOURCETYPE_ANY, NULL, &net_rsc, &net_rsc_enum) == NO_ERROR){

		// Get all network resources
		unsigned long int net_rsc_count = 0xFFFFFFFF;
		unsigned long int minimum_net_rsc_size = 0x00004000;
		NETRESOURCEA* net_rsc = reinterpret_cast<NETRESOURCEA*>(new char[minimum_net_rsc_size]{});
		if(WNetEnumResourceA(net_rsc_enum, &net_rsc_count, net_rsc, &minimum_net_rsc_size) == NO_ERROR){

			// Convert NETRESOURCEA to NetworkResource and add it to the list of network resources
			network_resources.reserve(net_rsc_count);
			for(unsigned int net_rsc_indx = 0; net_rsc_indx < net_rsc_count; net_rsc_indx++){
				network_resources.push_back(NetworkResource(net_rsc[net_rsc_indx]));
			}
		}

		// Free temporary buffer
		delete[] net_rsc;

		// Close enumerating handle
		static_cast<void>(WNetCloseEnum(net_rsc_enum));
	}

	return network_resources;
}