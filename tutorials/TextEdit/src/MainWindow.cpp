/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiCSS.h"
#include "nuiVBox.h"


/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
  SetDebugMode(true);
  //mClearBackground = false;
}

MainWindow::~MainWindow()
{
}


#include "nuiUnicode.h"
void Test(const nglString& txt)
{
  nuiTextRangeList ranges;
  nuiSplitText(txt, ranges, nuiST_ScriptChange);
  
  nuiTextRangeList::iterator it = ranges.begin();
  nuiTextRangeList::iterator end = ranges.end();
  uint32 i = 0;
  uint32 pos = 0;
  while (it != end)
  {
    const nuiTextRange& range(*it);
    uint32 len = range.mLength;
    printf("range %d (%d - %d) (%s - %s)\n%s\n", i, pos, len, nuiGetUnicodeScriptName(range.mScript).GetChars(), nuiGetUnicodeRangeName(range.mRange).GetChars(), txt.Extract(pos, len).GetChars());
    
    pos += len;
    ++i;
    ++it;
  }
  
}

class nuiTextRun
{
public:
  nuiTextRun(nuiFont* pFont, const nglString& rString, int32 Position, int32 Length)
  : mpFont(pFont),
    mString(rString),
    mPosition(Position),
    mLength(Length)
  {
  }

private:
  friend class nuiTextLayout;
  nuiFont* mpFont;
  const nglString& mString;
  int32 mPosition;
  int32 mLength;
  
  void AddGlyph(nuiFont* pFont, int32 Index, float X, float Y)
  {
    nuiGlyphLayout glyph;
    glyph.X = X;
    glyph.Y = Y;
    pFont->PrepareGlyph(Index, glyph, true);
    mGlyphs.push_back(glyph);
  }
  
private:
  std::vector<nuiGlyphLayout> mGlyphs;
};

class nuiTextLine
{
public:
  nuiTextLine(float X, float Y)
  {
  }
  
  virtual ~nuiTextLine()
  {
  }
  
  /** @name Drawing the Line */
  //@{
  void Draw(nuiDrawContext* pContext)
  {
  }
  //@}
  
  const std::vector<nuiTextRun*>& GetGlyphRuns() const
  {
  }
  
  float GetX() const
  {
    return mX;
  }
  
  float GetY() const
  {
    return mY;
  }
  
  void SetPosition(float X, float Y)
  {
    mX = X;
    mY = Y;
  }
private:
  friend class nuiTextLayout;
  
  void AddRun(Run* pRun)
  {
    mRuns.push_back(pRun);
  }
  
  std::vector<Run*> mRuns;
  float mX, mY;
};



class nuiTextLayout
{
public:
  nuiTextLayout(nuiFont* pFont)
  {
    pFont->Acquire();
    mpFonts.push_back(pFont);
  }
  
  virtual ~nuiTextLayout()
  {
    for (uint32 i = 0; i < mpFonts.size(); i++)
      mpFonts[i]->Release();
  }
  
private:
  std::vector<nuiFont*> mpFonts;
};

void MainWindow::OnCreation()
{
  nuiScrollView* pScrollView = new nuiScrollView;
  nuiEditText* pText = new nuiEditText(_T("Type something here\n\n"));
  
  nglIStream* pStream = nglPath("rsrc:/test.txt").OpenRead();
  if (pStream)
  {
    pStream->SetTextEncoding(eUTF8);
    nglString text;
    pStream->ReadText(text);
    Test(text);
    pText->AddText(text);
    delete pStream;
  }
  
  pText->SetTextColor(nuiColor(128,128,128));
  AddChild(pScrollView);
  pScrollView->AddChild(pText);
  
  pScrollView->SetBorder(10, 10, 32, 32);
}
