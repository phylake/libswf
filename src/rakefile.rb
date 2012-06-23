OUTPUT = 'main.out'

# rule '.o' => ['.cpp','h'] do |t|
rule '.o' => ['.cpp'] do |t|
  sh %{g++ -c #{t.source}}
end

file OUTPUT => :objects do
  sh %{g++ -Wall #{Dir['*.o'].join(' ')} -lz -o #{OUTPUT}}
end

task :objects => Dir['*.cpp'].map!{|f| f.split('.')[0] + '.o'}
task :build => OUTPUT

task :clean do
  `rm *.o`
  `rm #{OUTPUT}`
end

task :default => :build
