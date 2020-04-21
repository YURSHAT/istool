// Update.h: interface for the CUpdate class.

#pragma once

class CUpdate {
public:
	CUpdate() {
		m_pNext = m_pFirst;
		m_pFirst = this;
	}

	virtual ~CUpdate() {
		CUpdate* p = m_pFirst;
		CUpdate* pSave = NULL;
		while(p) {
			if(p==this) {
				if(pSave)
					pSave->m_pNext = m_pNext;
				else
					m_pFirst = m_pNext;
				break;
			}
			pSave = p;
			p = p->m_pNext;
		}
	}

	enum Hint {
		// General
		HINT_INITIALIZE = 0,		// pParam: bool EnableISX
		HINT_INITVIEW,
		HINT_APPLYCHANGES
	};

	static void UpdateAll(Hint lHint=HINT_INITIALIZE,void* pParam=NULL) {
		CUpdate* p = m_pFirst;
		while(p) {
			p->OnUpdate(lHint,pParam);
			p = p->m_pNext;
		}
	}

	void UpdateOther(Hint lHint=HINT_INITIALIZE,void* pParam=NULL) {
		CUpdate* p = m_pFirst;
		while(p) {
			if(p!=this) p->OnUpdate(lHint,pParam);
			p = p->m_pNext;
		}
	}

protected:
	virtual void OnUpdate(LONG lHint,void* pParam) = 0;
private:
	static CUpdate*	m_pFirst;
	CUpdate* m_pNext;
};
