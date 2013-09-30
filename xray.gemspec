# Describe your gem and declare its dependencies:
specification = Gem::Specification.new do |s|
  s.name = "xray"
  s.summary = "Dump backtrace for all threads."
  s.version = "1.1.2"
  s.author = "Brian Howald"
  s.email = "bhowald@gmail.com"
  s.rubyforge_project = 'xray'
  s.platform = Gem::Platform::RUBY
  s.executables = FileList['bin/**/*'].collect {|path| path.sub /bin\//, ""}
  s.files = FileList['lib/**/*.rb'] + FileList['test/**/*.rb'] + FileList['examples/**/*'] +
            FileList['**/*.diff'] + [ "rails/init.rb"]
            [ 'gdb_macros', 'Rakefile' ]
  s.require_path = "lib"
  s.extensions = []
  s.rdoc_options << '--title' << 'XRay' << '--main' << 'README' << '--line-numbers'
  s.has_rdoc = true
  s.extra_rdoc_files = ['README']
  s.test_file = "test/all_tests.rb"
end
