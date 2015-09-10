
// DiskStatisticsDoc.h : interface of the CDiskStatisticsDoc class
//


#pragma once

class CLeftView;
class CFileView;

class CDiskStatisticsDoc : public CDocument
{
protected: // create from serialization only
	CDiskStatisticsDoc();
	DECLARE_DYNCREATE(CDiskStatisticsDoc)

// Attributes
public:

	
// Operations
public:

public:
	CLeftView *pLeftView;
    CFileView *pFileView;

// Overrides
public:
//	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CDiskStatisticsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnNewDocument();
};
