from pick import pick
import os
import git
from git import RemoteProgress
import shutil

data = {
    'language': None,
    'os': None,
    'version': None,
    'path': None,
}

def selectData(title, options, indicator='>'):
    option, index = pick(options, title, indicator='>')
    
    return option


if __name__ == '__main__':
    data['language'] = selectData("Please choose a language:", ['English', 'Spanish'])
    data['version'] = selectData("Please choose a version:", ['v1', 'v0'])
    data['os'] = selectData(
        "Please choose an OS:",
        [
            'Ubuntu 20.04',
            'Ubuntu 22.04',
            'Ubuntu 24.04',
            'CentOS 7',
            'CentOS 8',
            'Debian 10',
            'Debian 11',
        ]
    )
    while True:
        path = str(input("Please enter the path where Paymenter is installed. [/var/www/paymenter]: "))
        if path.strip() == '': path = '/var/www/paymenter'
        data['path'] = path

        if os.path.exists(path):
            break
        print("Path does not exist. Please try again.")

    class CloneProgress(RemoteProgress):
        def update(self, op_code, cur_count, max_count=None, message=""):
            if message:
                print(message)
    
    print("Cloning into %s" % '/tmp/Pelican-Paymenter')
    
    git.Repo.clone_from(
        'https://github.com/MayiVT/Pelican-Paymenter',
        '/tmp/Pelican-Paymenter'
    )

    shutil.move(
        f'/tmp/Pelican-Paymenter/src/{data["version"]}',
        f'{data["path"]}/extensions/Servers'
    )
