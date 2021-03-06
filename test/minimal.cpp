#include <systemc>

using namespace sc_dt;
using namespace sc_core;
using namespace std;

SC_MODULE(writer) {
  sc_out<                    sc_int<8>> out
    sc_out<                    sc_int<8>> out1;
  sc_out<                    sc_int<8>> out2;

  int val = 0;
  void compute(void) {
    while (true) {
      wait(1, SC_NS);
      val = (val + 1);
      cout << name() << ": sending " << val << endl;
      out.write(val);
    }
  }
  SC_CTOR(writer) {

    //-MemberExpr 0x664d108 <line:428:7, col:42> '<bound member function type>' ->create_thread_process 0x5ab8e50
    //-MemberExpr 0x66a54f0 <line:417:6, col:41> '<bound member function type>' ->create_method_process 0x5ab8b08

    SC_THREAD(compute);
  }
};

SC_MODULE(receiver) {
  sc_in<sc_int<8>> in;
  sc_signal<sc_int<8>> sin;
  sc_signal<sc_int<8>> swindow;

  void compute(void) {
    sin = in.read();
    swindow = 0;
    if (sin.read() > 10) {
      swindow = sin;
    }
    std::cout << name() << ": in rx " << sin << endl;
    std::cout << name() << ": window rx " << swindow << endl;
  }

  SC_CTOR(receiver) {
    SC_METHOD(compute);
    sensitive << in;
  }
};

SC_MODULE(receiver2) {
  sc_in<sc_int<8>> in;
  sc_signal<sc_int<8>> sin;
  sc_signal<sc_int<8>> swindow;

  void compute(void) {
    sin = in.read();
    swindow = 0;
    if (sin.read() > 10) {
      swindow = sin;
    }
    std::cout << name() << ": in rx " << sin << endl;
    std::cout << name() << ": window rx " << swindow << endl;
  }

  void c1(){}
  void c2(){}
  SC_CTOR(receiver2) {
    SC_METHOD(compute);
    sensitive << in;
    SC_THREAD(c1);
  }
};

class top : public sc_module {

  sc_signal<sc_int<8>> sig;
  writer w;
  receiver r;
  receiver *r1;

  SC_HAS_PROCESS(top);

public:
  top(sc_module_name name_) : w("writer"), r("receiver") {
    r1 = new receiver("receiver1");
    w.out.bind(sig);
    r.in.bind(sig);
    //r1->in.bind(sig);
  }
};

int sc_main(int argc, char **argv) {

  top t("top");

  sc_start(100, SC_NS);
  return 0;
}
