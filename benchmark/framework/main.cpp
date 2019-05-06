#include "levelgraph.hpp"
#include <CLI11.hpp> 

int main(int argc, char* argv[]) {

  CLI::App app{"Wavefront"};

  unsigned num_threads {1}; 
  app.add_option("-t,--num_threads", num_threads, "number of threads (default=1)");

  unsigned num_rounds {1};  
  app.add_option("-r,--num_rounds", num_rounds, "number of rounds (default=1)");

  std::string model = "tf";
  app.add_option("-m,--model", model, "model name tbb|omp|tf (default=tf)")
     ->check([] (const std::string& m) {
        if(m != "tbb" && m != "omp" && m != "tf") {
          return "model name should be \"tbb\", \"omp\", or \"tf\"";
        }
        return "";
     });

  CLI11_PARSE(app, argc, argv);
   
  std::cout << "model=" << model << ' '
            << "num_threads=" << num_threads << ' '
            << "num_rounds=" << num_rounds << ' '
            << std::endl;


  const int width {12};
  const int repeat [] = {1, 5, 10, 100};
  
  std::cout << std::setw(width) << "|V|+|E|"
            << std::setw(width) << repeat[0]
            << std::setw(width) << repeat[1]
            << std::setw(width) << repeat[2]
            << std::setw(width) << repeat[3]
            << '\n';

  std::cout.precision(3);

  for(int i=1; i<=451; i += 15) {
    for(int k=0; k<4; k++) {
      double runtime {0.0};

      LevelGraph graph(i, i);

      if(k == 0) {
        std::cout << std::setw(width) << graph.graph_size();
      }
      
      for(unsigned j=0; j<num_rounds; ++j) {
        if(model == "tf") {
          runtime += measure_time_taskflow(graph, num_threads, repeat[k]).count();
        }
        else if(model == "tbb") {
          runtime += measure_time_tbb(graph, num_threads, repeat[k]).count();
        }
        else if(model == "omp") {
          runtime += measure_time_omp(graph, num_threads, repeat[k]).count();
        }
        else assert(false);
      }
              
      std::cout << std::setw(width) << runtime / num_rounds / 1e3 << std::fixed;
    }
    std::cout << std::endl;
  }

}


