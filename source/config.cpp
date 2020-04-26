#include "config.hpp"

cconfig config;

void cconfig::run( const char *name )
{
	PWSTR path_to_documents;
	if ( SUCCEEDED( LI_FN( SHGetKnownFolderPath ).safe_cached( )( FOLDERID_Documents, 0, nullptr, &path_to_documents ) ) )
	{
		path = path_to_documents;
		path /= name;
		LI_FN( CoTaskMemFree ).safe_cached( )( path_to_documents );
	}

	if ( !std::filesystem::is_directory( path ) )
	{
		std::filesystem::remove( path );
		std::filesystem::create_directory( path );
	}

	std::transform( std::filesystem::directory_iterator { path },
		std::filesystem::directory_iterator { },
		std::back_inserter( configs ),
		[ ]( const auto &entry ) { return entry.path( ).filename( ).string( ); } );
}

void cconfig::load( size_t id )
{
	if ( !std::filesystem::is_directory( path ) )
	{
		std::filesystem::remove( path );
		std::filesystem::create_directory( path );
	}

	std::ifstream in { path / configs[ id ] };

	if ( !in.good( ) )
		return;

	ArchiveX<std::ifstream>{ in } >> clicker;
	in.close( );
}

void cconfig::save( size_t id ) const
{
	if ( !std::filesystem::is_directory( path ) )
	{
		std::filesystem::remove( path );
		std::filesystem::create_directory( path );
	}

	std::ofstream out { path / configs[ id ] };

	if ( !out.good( ) )
		return;

	ArchiveX<std::ofstream>{ out } << clicker;
	out.close( );
}

void cconfig::add( const char *name )
{
	if ( *name && std::find( std::cbegin( configs ), std::cend( configs ), name ) == std::cend( configs ) )
		configs.emplace_back( name );
}

void cconfig::remove( size_t id )
{
	std::filesystem::remove( path / configs[ id ] );
	configs.erase( configs.cbegin( ) + id );
}

void cconfig::rename( size_t item, const char *newName )
{
	std::filesystem::rename( path / configs[ item ], path / newName );
	configs[ item ] = newName;
}

void cconfig::reset( )
{
	clicker = { };
}