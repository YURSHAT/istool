/*----------------------------------------------------------------------.
Copyright (c) 1998,1999 Gipsysoft. All Rights Reserved.
File:	SplitPath.h
Owner:	russf@gipsysoft.com
Purpose:	Convientient wrapper for the split/make path functions.
					Also adds some checking when setting the path components
					Not yet UNICODE
					All inline.
----------------------------------------------------------------------*/
#ifndef SPLITPATH_H
#define SPLITPATH_H

#ifndef _INC_STDLIB
	#include <stdlib.h>
#endif	//	_INC_STDLIB

class CSplitPath
{
public:
	inline CSplitPath( const char * pcszSplit = NULL );
	inline void Split( const char * pcszSplit );
	inline void Make( char *szPathBuffer );

	inline const char *GetDrive() const { return m_szDrive; }
	inline const char *GetDirectory() const { return m_szDirectory; }
	inline const char *GetFilename() const { return m_szFilename; }
	inline const char *GetExtension() const { return m_szExtension; }

	inline void SetDrive( const char *pcszDrive );
	inline void SetDirectory( const char *pcszDirectory );
	inline void SetFilename( const char *pcszFilename );
	inline void SetExtension( const char *pcszExtension );

	char m_szDrive[ _MAX_DRIVE ];
	char m_szDirectory[ _MAX_DIR ];
	char m_szFilename[ _MAX_FNAME ];
	char m_szExtension[ _MAX_EXT ];
};


inline CSplitPath::CSplitPath( LPCTSTR pcszSplit /*= NULL*/ )
	{
		if( pcszSplit )
			Split( pcszSplit );
	}


inline void CSplitPath::Split( const char * pcszSplit )
	{ ATLASSERT( pcszSplit );_splitpath( pcszSplit, m_szDrive, m_szDirectory, m_szFilename, m_szExtension ); }

inline void CSplitPath::Make( char *szPathBuffer )
	{ _makepath( szPathBuffer, m_szDrive, m_szDirectory, m_szFilename, m_szExtension ); }

inline void CSplitPath::SetDrive( const char *pcszDrive )
	{ ATLASSERT( _tcslen( pcszDrive ) < sizeof( m_szDrive ) ); _tcscpy( m_szDrive, pcszDrive ); }

inline void CSplitPath::SetDirectory( const char *pcszDirectory )
	{ ATLASSERT( _tcslen( pcszDirectory ) < sizeof( m_szDirectory ) ); _tcscpy( m_szDirectory, pcszDirectory ); }

inline void CSplitPath::SetFilename( const char *pcszFilename )
	{ ATLASSERT( _tcslen( pcszFilename ) < sizeof( m_szFilename ) ); _tcscpy( m_szFilename, pcszFilename ); }

inline void CSplitPath::SetExtension( const char *pcszExtension )
	{ ATLASSERT( _tcslen( pcszExtension ) < sizeof( m_szExtension ) ); _tcscpy( m_szExtension, pcszExtension ); }

#endif //SPLITPATH_H
