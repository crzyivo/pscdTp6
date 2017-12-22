#include <mutex>
#include <condition_variable>

class CierreServidor{
private:
    std::mutex m;
    std::condition_variable leer,escribir;
    int clientesConectados;
    bool leyendo = false;
    bool escribiendo = false;
    
public:
    
    void addAbiertos(){
        std::unique_lock<std::mutex> lk(m);
        if(leyendo || escribiendo){
        escribir.wait(lk);
        }
        escribiendo = true;
        clientesConectados++;
        escribiendo = false;
        escribir.notify_one();
        leer.notify_all();
    }
    
    int addCerrados(){
        std::unique_lock<std::mutex> lk(m);
        if(leyendo || escribiendo){
        escribir.wait(lk);
        }
        escribiendo = true;
        clientesConectados--;
        int c = clientesConectados;
        escribiendo = false;
        escribir.notify_one();
        leer.notify_all();
        return c;
    }
    bool ultimaConexion(){
        std::unique_lock<std::mutex> lk(m);
        if(escribiendo){
        leer.wait(lk);
        }
        leyendo=true;
        if(clientesConectados==0){
            leyendo=false;
            escribir.notify_one();
            return true;
        }else{
            leyendo=false;
            escribir.notify_one();
            return false;
        }
    }
    int getConectados(){
        std::unique_lock<std::mutex> lk(m);
        if(escribiendo){
        leer.wait(lk);
        }
        leyendo=true;
        int c = clientesConectados;
        leyendo = false;
        escribir.notify_one();
        return c;
    }
}; 
