// glimTestDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "glimTest.h"
#include "glimTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

    // 구현입니다.
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CglimTestDlg 대화 상자
CglimTestDlg::CglimTestDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_GLIMTEST_DIALOG, pParent),
    m_hIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME))
{
    //m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CglimTestDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CglimTestDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_IMAGE, &CglimTestDlg::OnBnClickedBtnImage)
    ON_BN_CLICKED(IDC_BTN_ACTION, &CglimTestDlg::OnBnClickedBtnAction)
END_MESSAGE_MAP()

// CglimTestDlg 메시지 처리기
BOOL CglimTestDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // 이 대화 상자의 아이콘을 설정합니다.
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    return TRUE;
}

void CglimTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

void CglimTestDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 아이콘을 그립니다.
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CglimTestDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CglimTestDlg::OnBnClickedBtnImage()
{
    int nWidth = 640;
    int nHeight = 480;
    int nBpp = 8;

    m_image.Create(nWidth, -nHeight, nBpp);
    if (nBpp == 8) {
        static RGBQUAD rgb[256];
        for (int i = 0; i < 256; i++)
            rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
        m_image.SetColorTable(0, 256, rgb);
    }

    int nPitch = m_image.GetPitch();
    unsigned char* fm = (unsigned char*)m_image.GetBits();

    memset(fm, 0xff, nWidth * nHeight);

    UpdateDisplay();
}

void CglimTestDlg::UpdateDisplay()
{
    CClientDC dc(this);
    m_image.Draw(dc, 0, 0);
}

void CglimTestDlg::moveRect()
{
    int nGray = 80;
    int nWidth = m_image.GetWidth();
    int nHeight = m_image.GetHeight();
    int nPitch = m_image.GetPitch();
    int nRadius = GetDlgItemInt(IDC_INPUT_RADIUS);
    int nSttX = rand() % (nWidth - 2 * nRadius);
    int nSttY = rand() % (nHeight - 2 * nRadius);
    unsigned char* fm = (unsigned char*)m_image.GetBits();
    int nSize = 20;

    RedrawWindow();

    memset(fm, 0xff, nWidth * nHeight);

    drawCircle(fm, nSttX, nSttY, nRadius, nGray);

    drawCross(fm, nSttX + nRadius, nSttY + nRadius, nSize, nGray);

    CPoint ptCenter = findCenter();

    drawYellowCircle(ptCenter, nRadius);
}

void CglimTestDlg::OnBnClickedBtnAction()
{
    moveRect();
}

BOOL CglimTestDlg::validImgPos(int x, int y)
{
    int nWidth = m_image.GetWidth();
    int nHeight = m_image.GetHeight();

    CRect rect(0, 0, nWidth, nHeight);

    return rect.PtInRect(CPoint(x, y));
}

void CglimTestDlg::drawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray)
{
    CClientDC dc(this);

    int nCenterX = x + nRadius;
    int nCenterY = y + nRadius;
    int nPitch = m_image.GetPitch();

    for (int j = y; j < y + nRadius * 2; j++) {
        for (int i = x; i < x + nRadius * 2; i++) {
            if (isInCircle(i, j, nCenterX, nCenterY, nRadius))
                fm[j * nPitch + i] = nGray;
        }
    }

    UpdateDisplay();
}

bool CglimTestDlg::isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius)
{
    bool bRet = false;
    double dX = i - nCenterX;
    double dY = j - nCenterY;
    double dDist = dX * dX + dY * dY;

    if (dDist <= nRadius * nRadius && dDist >= (nRadius - 5) * (nRadius - 5)) {
        bRet = true;
    }

    return bRet;
}

void CglimTestDlg::drawCross(unsigned char* fm, int x, int y, int nSize, int nColor)
{
    int nPitch = m_image.GetPitch();
    int nWidth = m_image.GetWidth();
    int nHeight = m_image.GetHeight();

    for (int i = x - nSize / 2; i < x + nSize / 2; i++) {
        if (i >= 0 && i < nWidth) {
            fm[y * nPitch + i] = nColor;
        }
    }

    for (int j = y - nSize / 2; j < y + nSize / 2; j++) {
        if (j >= 0 && j < nHeight) {
            fm[j * nPitch + x] = nColor;
        }
    }

    UpdateDisplay();
}

CPoint CglimTestDlg::findCenter()
{
    unsigned char* fm = (unsigned char*)m_image.GetBits();
    int nWidth = m_image.GetWidth();
    int nHeight = m_image.GetHeight();
    int nPitch = m_image.GetPitch();

    CRect rect(0, 0, nWidth, nHeight);
    int nSumX = 0;
    int nSumY = 0;
    int nCount = 1;
    for (int j = rect.top; j < rect.bottom; j++)
    {
        for (int i = rect.left; i < rect.right; i++)
        {
            if (fm[j * nPitch + i] != 0xff)
            {
                nSumX += i;
                nSumY += j;
                nCount++;
            }
        }
    }
    int nCenterX = nSumX / nCount;
    int nCenterY = nSumY / nCount;

    return CPoint(nCenterX, nCenterY);
}


void CglimTestDlg::drawYellowCircle(CPoint ptCenter, int nRadius)
{
    CClientDC dc(this);

    //원의 내부를 투명하게 하기 위하여 CBrush 준비
    CBrush brush;
    brush.CreateStockObject(NULL_BRUSH);
    CBrush* pOldBrush = dc.SelectObject(&brush);

    //원을 노란색으로 그리기 위해 CPen 준비
    CPen pen;
    pen.CreatePen(PS_SOLID, 5, RGB(0xff, 0xff, 0x00));
    CPen* pOldPen = dc.SelectObject(&pen);

    int nSpace = 10; //기존의 원과 거리를 두고 그리기 위한 공간
    dc.Ellipse(ptCenter.x - nRadius - nSpace, ptCenter.y - nRadius - nSpace,
        ptCenter.x + nRadius + nSpace, ptCenter.y + nRadius + nSpace);

    dc.SelectObject(pOldBrush);
    dc.SelectObject(pOldPen);
}