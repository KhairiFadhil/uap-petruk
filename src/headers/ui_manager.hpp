#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include <ncurses.h>
#include <vector>
#include <string>
#include <stdexcept>
#include "stock_data.hpp"
#include "regression.hpp"

class UIManager {
private:
    WINDOW* m_headerWin;       
    WINDOW* m_infoWin;         
    WINDOW* m_statusWin;       
    WINDOW* m_resultWin;      
    
    bool m_showPredictions;    

    void setupColors();
    void createWindows();
    void renderTopHeader();
    void renderInfo(const LinearRegression& regression, const std::vector<double>& predictions);
    void renderStatus();
    void LoadingState();
    void showRowDetails(const StockPoint& point);
    
public:
    UIManager() : m_headerWin(nullptr), 
                  m_infoWin(nullptr), 
                  m_statusWin(nullptr),
                  m_resultWin(nullptr),
                  m_showPredictions(true) {}
    
    ~UIManager() { cleanup(); }
    
    std::string selectFile();
    void init();
    void displayDataTable(const std::vector<StockPoint>& data);
    void cleanup();
    void renderAll(const std::vector<StockPoint>& data,
                   const LinearRegression& regression,
                   const std::vector<double>& predictions);
    void renderResult(const std::vector<StockPoint>& data,
                     const LinearRegression& regression,
                     const std::vector<double>& predictions);
    int waitForInput();
    bool processInput(int input);   
    void showError(const std::string& message);
    int showMainMenu();
    static const int KEY_QUIT = 'q';
};

#endif