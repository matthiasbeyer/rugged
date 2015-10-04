require "test_helper"

class OdbTest < Rugged::TestCase
  def setup
    @repo = FixtureRepo.from_rugged("testrepo.git")
  end

  def test_new
    refdb = Rugged::Odb.new()
    assert_instance_of Rugged::Odb, refdb
  end

  def test_add_backend
    refdb = Rugged::Odb.new()
    refdb.add_backend(Rugged::Odb::Backend::Loose.new(File.join(@repo.path, "objects"), -1, 0, 0, 0), 1)
  end

  def test_each
    refdb = Rugged::Odb.new()
    refdb.add_backend(Rugged::Odb::Backend::Loose.new(File.join(@repo.path, "objects"), -1, 0, 0, 0), 1)

    ids = []
    refdb.each { |id| ids << id }
    assert_equal 31, ids.length
  end
end
