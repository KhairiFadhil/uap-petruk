#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include <ncurses.h>
#include <vector>
#include <string>
#include <stdexcept>
#include "stock_data.hpp"
#include "regression.hpp"
struct Viewport {
    int indeksMulai;       
    int indeksSelesai;         
    double hargaMin;      
    double hargaMax;      
    double levelZoom;     
    
    Viewport() : indeksMulai(0), indeksSelesai(0), hargaMin(0), hargaMax(0), levelZoom(1.0) {}
};

class UIManager {
private:
    WINDOW* headerWin;       
    WINDOW* graphWin;        
    WINDOW* infoWin;         
    WINDOW* statusWin;       
    
    Viewport viewport;
    bool showPredictions;    
    
    // Setup warna-warna buat UI
    void setupColors();
    
    // Bikin semua window
    void createWindows();
    
    // Render header sama judul
    void renderHeader();
    
    // Render header utama (untuk semua screen)
    void renderTopHeader();
    
    // Render grafik utama (sumbu, data, regresi)
    void renderGraph(const std::vector<StockPoint>& data, 
                     const LinearRegression& regression,
                     const std::vector<double>& predictions);
    
    // Render panel info (parameter model, harga prediksi)
    void renderInfo(const LinearRegression& regression);
    
    // Render status bar (info kontrol)
    void renderStatus();
    
    // Gambar sumbu X sama Y
    void drawAxes();
    
    // Plot data historis
    void plotHistoricalData(const std::vector<StockPoint>& data);
    
    // Gambar garis regresi
    void drawRegressionLine(const LinearRegression& regression);
    
    // Untuk Loading State (Gimik)
    void LoadingState();
    
    // Navigasi tabel data CSV
    
    // Tampilkan detail row yang dipilih
    void showRowDetails(const StockPoint& point);
    // Plot prediksi masa depan
    void plotPredictions(const std::vector<double>& predictions);
    
    // Update viewport based navigasi
    void updateViewport(int direction);
    
    // Atur level zoom
    void adjustZoom(bool zoomIn);
    
public:
    UIManager() : headerWin(nullptr), graphWin(nullptr), 
                  infoWin(nullptr), statusWin(nullptr), showPredictions(true) {}
    
    ~UIManager() { cleanup(); }
    
    std::string selectFile();
    // Inisialisasi NCurses
    void init();
    // Tampilkan tabel data CSV
    void displayDataTable(const std::vector<StockPoint>& data);
    
    // Bersihin NCurses
    void cleanup();
    
    // Siapkan visualisasi (hitung skala grafik, setup grid)
    void prepareVisualization(const std::vector<StockPoint>& data,
                             const std::vector<double>& predictions);
    
    // Render semua komponen UI
    void renderAll(const std::vector<StockPoint>& data,
                   const LinearRegression& regression,
                   const std::vector<double>& predictions);
    
    // Tunggu input user
    int waitForInput();
    
    // Proses input user (return false buat exit)
    bool processInput(int input);
    
    // Tampilin pesan error
    void showError(const std::string& message);
    
    // Tombol-tombol buat kontrol
    static const int KEY_ZOOM_IN = '+';
    static const int KEY_ZOOM_OUT = '-';
    static const int KEY_TOGGLE_PRED = ' ';
    static const int KEY_QUIT = 'q';
};

#endif // UI_MANAGER_HPP 