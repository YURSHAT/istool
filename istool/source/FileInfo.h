// FileInfo.h: interface for the CFileInfo class.
// (c) Codeguru & friends
// Coded by Antonio Tejada Lacaci. 1999
// atejada@espanet.com
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CFileInfo {  
public:
   CFileInfo();
   /**
    * Copy constructor
    * @param finf CFileInfo to copy member variables from.
    */
   CFileInfo(const CFileInfo& finf);

   /**
    * Destructor
    */
   ~CFileInfo();

   /**
    * Initializes CFileInfo member variables.
    * @param pwfd Values to init member variables.
    * @param strPath Path of the file the CFileInfo refers to.
    * @param lParam User defined parameter.
    */
   void Create(const WIN32_FIND_DATA* pwfd, const CString strPath, LPARAM lParam=NULL);

   /**
    * Initializes CFileInfo member variables.
    * @param strFilePath Absolute path for file or directory
    * @param lParam User defined parameter.
    */
   void Create(const CString strFilePath, LPARAM lParam = NULL);
   
   /* Get File size info */
   /** @return File size in bytes as a DWORD value. */
   DWORD GetLength(void) const { return (DWORD) m_uhFileSize; };
   /** @return File size in bytes as an ULONGLONG value. */
   ULONGLONG GetLength64(void) const { return m_uhFileSize; };
   
   /* Get File split info (equivalent to CFindFile members) */
   /** @return C: for C:\WINDOWS\WIN.INI */
   CString GetFileDrive(void) const;
   /** @return \WINDOWS\ for C:\WINDOWS\WIN.INI */
   CString GetFileDir(void) const;
   /** @return WIN for C:\WINDOWS\WIN.INI */
   CString GetFileTitle(void) const;
   /** @return INI for C:\WINDOWS\WIN.INI */
   CString GetFileExt(void) const;
   /** @return C:\WINDOWS\ for C:\WINDOWS\WIN.INI */
   CString GetFileRoot(void) const { return GetFileDrive() + GetFileDir(); };
   /** @return WIN.INI for C:\WINDOWS\WIN.INI */
   CString GetFileName(void) const { return GetFileTitle() + GetFileExt(); };
   /** @return C:\WINDOWS\WIN.INI for C:\WINDOWS\WIN.INI */
   const CString& GetFilePath(void) const { return m_strFilePath; }

   /* Get File attributes info (equivalent to CFindFile members) */
   DWORD GetAttributes(void) const { return m_dwAttributes; };
   BOOL IsDirectory(void) const { return m_dwAttributes & FILE_ATTRIBUTE_DIRECTORY; };
   BOOL IsArchived(void) const { return m_dwAttributes & FILE_ATTRIBUTE_ARCHIVE; };
   BOOL IsReadOnly(void) const { return m_dwAttributes & FILE_ATTRIBUTE_READONLY; };
   BOOL IsCompressed(void) const { return m_dwAttributes & FILE_ATTRIBUTE_COMPRESSED; };
   BOOL IsSystem(void) const { return m_dwAttributes & FILE_ATTRIBUTE_SYSTEM; };
   BOOL IsHidden(void) const { return m_dwAttributes & FILE_ATTRIBUTE_HIDDEN; };
   BOOL IsTemporary(void) const { return m_dwAttributes & FILE_ATTRIBUTE_TEMPORARY; };
   BOOL IsNormal(void) const { return m_dwAttributes == 0; };
   
   LPARAM m_lParam;        /** User-defined parameter */
private:
   CString m_strFilePath;  /** Full filepath of file (directory+filename) */
   DWORD m_dwAttributes;   /** File attributes of file (as returned by FindFile() */
   ULONGLONG m_uhFileSize; /** File of size. (COM states LONGLONG as hyper, so "uh" means 
                          unsigned hyper) */
};

class CFileInfoArray : public CSimpleArray<CFileInfo> {
public:

   /**
    * Default constructor
    */
   CFileInfoArray();


   /**
    * Adds a file or all contained in a directory to the CFileInfoArray
    * Only "static" data for CFileInfo is filled (by default CRC and checksum are NOT 
    * calculated when inserting CFileInfos).
    * @param strDirName Name of the directory, ended in backslash.
    * @param strMask Mask of files to add in case that strDirName is a directory
    * @param bRecurse Wether to recurse or not subdirectories
    * @param lAddParam Parameter to pass to protected member function AddFileInfo
    * Allowed parameters for default implementation are:
    * <OL>
    *   <LI>AP_NOSORT     : Insert CFileInfos in a unordered manner
    *   <LI>AP_SORTBYSIZE : Insert CFileInfos ordered by uhFileSize (presumes array is 
    *   previously ordered by uhFileSize).
    *   <LI>AP_SORTBYNAME : Insert CFileInfos ordered by strFilePath (presumes array is 
    *   previously ordered by strFilePath)
    *   <LI>AP_SORTDESCENDING : If ored with AP_SORTBYSIZE or AP_SORTBYNAME, sort order will
    *   be descending (by default is ascending).
    * <OL>
    * @param bIncludeDirs Wether to add or not CFileInfos for directories
    * @param pbAbort Pointer to a variable to signal abort of directory retrieval 
    * (multithreaded apps).
    * @param pulCount Pointer to a variable incremented each time a CFileInfo is added to the
    * array (multithreaded apps).
    * @return The number of CFileInfos added to the array
    * @see AddFile
    * @see AddFileInfo
    */
   int AddDir(const CString strDirName, const CString strMask, BOOL bRecurse, 
      BOOL bIncludeDirs=FALSE, 
      const volatile BOOL* pbAbort = NULL, volatile ULONG* pulCount = NULL);

   /**
    * Adds a single file or directory to the CFileInfoArray. In case of directory, files
    * contained in the directory are NOT added to the array.
    * @param strFilePath Name of the file or directory to add. NOT ended with backslash.
    * @param lAddParam Parameter to pass to protected member function AddFileInfo.
    * @return The position in the array where the CFileInfo was added (-1 if CFileInfo 
    * wasn't added)
    * @see AddDir
    * @see AddFileInfo
    */
   int AddFile(const CString strFilePath, LPARAM lAddParam);

protected:

   /**
    * Called by AddXXXX to add a CFileInfo to the array. Can be overriden to:
    * 1. Add only desired CFileInfos (filter)
    * 2. Fill user param lParam
    * 3. Change sort order/criteria
    * Default implementation sorts by lAddParam values and adds all CFileInfos 
    * (no filtering)
    * @param finf CFileInfo to insert in the array.
    * @param lAddParam Parameter passed from AddDir function.
    * @return The position in the array where the CFileInfo was added or -1 if the CFileInfo 
    * wasn't added to the array.
    * @see AddDir
    */
   virtual int AddFileInfo(CFileInfo& finf);
};

/* Samples

This code adds all files in root directory and its subdirectories (but not directories themselves) to the array and TRACEs them: 

CFileInfoArray fia;

fia.AddDir(
   "C:\\",                                     // Directory
   "*.*",                                      // Filemask (all files)
   TRUE,                                       // Recurse subdirs
   fia::AP_SORTBYNAME | fia::AP_SORTASCENDING, // Sort by name and ascending
   FALSE                                       // Do not add array entries for directories (only for files)
);
TRACE("Dumping directory contents\n");
for (int i=0;i<fia.GetSize();i++) TRACE(fia[i].GetFilePath()+"\n");

You can also call AddDir multiple times. The example shows files in root directories (but not subdirectories) of C:\\ and D:\\: 

CFileInfoArray fia;

// Note both AddDir use the same sorting order and direction
fia.AddDir("C:\\", "*.*", FALSE, fia::AP_SORTBYNAME | fia::AP_SORTASCENDING, FALSE );
fia.AddDir("D:\\", "*.*", FALSE, fia::AP_SORTBYNAME | fia::AP_SORTASCENDING, FALSE );
TRACE("Dumping directory contents for C:\\ and D:\\ \n");
for (int i=0;i<fia.GetSize();i++) TRACE(fia[i].GetFilePath()+"\n");


Or you can add individual files: 

CFileInfoArray fin;

// Note both AddDir and AddFile must use the same sorting order and direction
fia.AddDir("C:\\WINDOWS\\", "*.*", FALSE, fia::AP_SORTBYNAME | fia::AP_SORTDESCENDING, FALSE );
fia.AddFile("C:\\AUTOEXEC.BAT", fia::AP_SORTBYNAME | fia::SORTDESCENDING);
TRACE("Dumping directory contents for C:\\WINDOWS\\ and file C:\\AUTOEXEC.BAT\n");
for (int i=0;i<fia.GetSize();i++) TRACE(fia[i].GetFilePath()+"\n");

And mix directories with individual files: 

CFileInfoArray fin;

// Note both AddDir and AddFile must use the same sorting order and direction
// Note also the list of filemasks *.EXE and *.COM
fia.AddDir("C:\\WINDOWS\\", "*.EXE;*.COM", FALSE, fia::AP_SORTBYNAME | fia::AP_SORTDESCENDING, FALSE );
fia.AddFile("C:\\AUTOEXEC.BAT", fia::AP_SORTBYNAME | fia::SORTDESCENDING);
// Note no trailing bar for next AddFile (we want to insert an entry for the directory
// itself, not for the files inside the directory)
fia.AddFile("C:\\PROGRAM FILES", fia::AP_SORTBYNAME | fia::SORTDESCENDING);
TRACE("Dumping directory contents for C:\\WINDOWS\\, file C:\\AUTOEXEC.BAT and "
" directory \"C:\\PROGRAM FILES\" \n");
for (int i=0;i<fia.GetSize();i++) TRACE(fia[i].GetFilePath+"\n");

*/
