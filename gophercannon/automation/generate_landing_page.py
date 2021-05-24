import glob, os
import git
from jinja2 import Template

repo = git.Repo(search_parent_directories=True)
REPO_HASH = repo.head.object.hexsha
BASE_COMMIT_URL = "https://github.com/gopher-motorsports/gophercan-lib/commit/"
LANDING_PAGE_NAME = "landing_page.html"

os.chdir("reports")
network_reports = glob.glob("*.html")
os.chdir("..")

network_reports = [x for x in network_reports if x != LANDING_PAGE_NAME]

urls = []

for network in network_reports:
    urls.append({ 'name' : network[0:network.find('_report.html')], 'url' : network })

filename = 'landing_page_TEMPLATE.html.jinja2'
with open(os.path.join('templates', filename)) as file_:
    template = Template(file_.read())
    output = template.render(   repo_hash=REPO_HASH[0:6],
                                commit_url=BASE_COMMIT_URL + REPO_HASH,
                                urls=urls)
    filename = "landing_page.html"
    with open(os.path.join('reports', filename), "w") as fh:
        fh.write(output)
